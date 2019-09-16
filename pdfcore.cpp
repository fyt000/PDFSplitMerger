#include "pdfcore.h"

#include <PDFPage.h>
#include <PDFPageInput.h>
#include <PDFWriter.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace PDFCore {

bool merge(std::string &errMsg, const std::vector<std::string> &files, const std::string &dest) {
    PDFWriter pdfWriter;
    try {
        ExWrapper([&]() { return pdfWriter.StartPDF(dest, ePDFVersionMax); },
                  "failed to start pdf " + dest);
        for (const auto &file : files) {
            ExWrapper([&]() { return pdfWriter.AppendPDFPagesFromPDF(file, PDFPageRange{}).first; },
                      "failed to merge " + file);
        }
    } catch (PDFException &ex) {
        errMsg = ex.what();
        return false;
    }
    return BoolWrapper([&]() { return pdfWriter.EndPDF(); }, "failed to end pdf " + dest, errMsg);
}

// int to string then add zeroes to match the length of c
// this helps viewing files sorted by filename (so _020 is placed after _019 instead of _1)
std::string intZeroPadding(unsigned long i, unsigned long count) {
    auto iDigit = static_cast<unsigned long>(log10(i)) + 1;
    auto cDigit = static_cast<unsigned long>(log10(count)) + 1;
    std::stringstream ss{};
    while (iDigit != cDigit) {
        ss << "0";
        iDigit += 1;
    }
    ss << i;
    return ss.str();
}

void writeOnePage(const std::string &srcPath, const std::string &filename, PDFRectangle rect,
                  unsigned long pageNum) {
    PDFWriter pdfWriter{};
    PDFPage page{};
    ExWrapper([&]() { return pdfWriter.StartPDF(filename, ePDFVersionMax); },
              "failed to start pdf for " + filename);
    page.SetMediaBox(rect);
    PDFPageRange singlePageRange{};
    singlePageRange.mType = PDFPageRange::eRangeTypeSpecific;
    singlePageRange.mSpecificRanges = {ULongAndULong{pageNum, pageNum}};
    std::string pageStr = std::to_string(pageNum);
    ExWrapper([&]() { return pdfWriter.MergePDFPagesToPage(&page, srcPath, singlePageRange); },
              "failed to split page: " + pageStr);
    ExWrapper([&]() { return pdfWriter.WritePage(&page); }, "failed to write page: " + pageStr);
    ExWrapper([&]() { return pdfWriter.EndPDF(); }, "failed to end page: " + pageStr);
    // Unclear if we should always EndPDF
}

bool split(std::string &errMsg, const std::string &srcPath, const std::string &destDir,
           const std::string &baseFileName, bool splitInMiddle) {
    // https://github.com/galkahana/PDF-Writer/wiki/PDF-Parsing

    InputFile pdfFile{};
    ExWrapper([&]() { return pdfFile.OpenFile(srcPath); }, "failed to open pdf");
    PDFParser parser{};
    auto inStream = pdfFile.GetInputStream();
    ExWrapper([&]() { return parser.StartPDFParsing(inStream); }, "failed to parse pdf");

    auto pageNum = parser.GetPagesCount();
    if (!pageNum) {
        errMsg = "pdf is empty or failed to parse content";
        return false;
    }

    errMsg = "";
    bool ret = true;
    for (unsigned long i = 0; i < pageNum; i++) {
        auto paddedStr = intZeroPadding(i + 1, pageNum);
        std::string fullDest = destDir + '/' + baseFileName + "_" + paddedStr;
        try {
            auto pageParsed = parser.ParsePage(i);
            if (!pageParsed) {
                throw PDFException("failed to parse page " + paddedStr);
            }
            PDFPageInput pageInput{&parser, pageParsed};
            auto rect = pageInput.GetMediaBox();
            if (splitInMiddle) {
                PDFRectangle leftRect{rect};
                leftRect.UpperRightX = rect.UpperRightX / 2;
                writeOnePage(srcPath, fullDest + "_A.pdf", leftRect, i);
                PDFRectangle rightRect{rect};
                rightRect.LowerLeftX = rect.UpperRightX / 2;
                writeOnePage(srcPath, fullDest + "_B.pdf", rightRect, i);
            } else {
                writeOnePage(srcPath, fullDest + ".pdf", rect, i);
            }
        } catch (PDFException &ex) {
            errMsg += ex.what();
            errMsg += "\n";
            ret = false;
        }
    }
    return ret;
}

} // namespace PDFCore
