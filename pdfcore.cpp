#include "pdfcore.h"
#include <sstream>
#include <cmath>
#include <iostream>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PDFWriter.h>
#include <PDFPageInput.h>

PDFCore::PDFCore(){
	errorMsg="None";
}

int PDFCore::merge(std::vector<std::string> &files, std::string dest){
	PDFWriter pdfWriter;

	EStatusCode status;

	status=pdfWriter.StartPDF(dest, ePDFVersion13);

	if (status!=PDFHummus::eSuccess){
		errorMsg="failed to start pdf "+dest;
		return status;
	}

	for (unsigned int i=0;i<files.size();i++){
		status=pdfWriter.AppendPDFPagesFromPDF(files[i], PDFPageRange()).first;
		if (status!=PDFHummus::eSuccess){
			errorMsg="failed to merge "+files[i]+"\n";
			break;
		}
	}
	status=pdfWriter.EndPDF();
	errorMsg="";
	if (status!=PDFHummus::eSuccess)
		errorMsg+="failed to end pdf "+dest;

	return status;
}


int PDFCore::split(std::string srcPath,std::string destDir,std::string baseFileName,bool split){
	//https://github.com/galkahana/PDF-Writer/wiki/PDF-Parsing
	EStatusCode status;
	InputFile pdfFile;
	PDFParser parser;
	status=pdfFile.OpenFile(srcPath);
	if (status!=PDFHummus::eSuccess){
		errorMsg="failed to open pdf";
		return status;
	}
	status=parser.StartPDFParsing(pdfFile.GetInputStream());
	if (status!=PDFHummus::eSuccess){
		errorMsg="failed to parse pdf";
		return status;
	}

	unsigned int long pageNum=parser.GetPagesCount();
	if (!pageNum){
		errorMsg="pdf is empty or failed to parse content";
		return PDFHummus::eFailure;
	}
	errorMsg="";

	for (unsigned int long i=0;i<pageNum;i++){
		std::string
			fullDest=destDir+'/'+baseFileName+"_"+intZeroPadding(i+1,pageNum);
		PDFPageInput pageInput(&parser,parser.ParsePage(i));
		PDFRectangle mediaBox=pageInput.GetMediaBox();

		if (split){
			PDFPageRange singlePageRange;
			singlePageRange.mType = PDFPageRange::eRangeTypeSpecific;
			singlePageRange.mSpecificRanges.push_back(ULongAndULong(i,i));

			PDFWriter pdfWriter1;
			PDFPage* p1=new PDFPage();
			status=pdfWriter1.StartPDF(fullDest+"_A.pdf", ePDFVersion13);

			if (status!=PDFHummus::eSuccess){
				goto error;
			}

			PDFRectangle leftRect=mediaBox;
			leftRect.UpperRightX=leftRect.UpperRightX/2;
			p1->SetMediaBox(leftRect);

			status=pdfWriter1.MergePDFPagesToPage(p1,srcPath,singlePageRange);
			if (status!=PDFHummus::eSuccess)
				goto error;

			status=pdfWriter1.WritePageAndRelease(p1);
			if (status!=PDFHummus::eSuccess)
				goto error;

			status=pdfWriter1.EndPDF();
			if (status!=PDFHummus::eSuccess)
				goto error;


			PDFWriter pdfWriter2;
			PDFPage* p2=new PDFPage();

			status=pdfWriter2.StartPDF(fullDest+"_B.pdf", ePDFVersion13);
			if (status!=PDFHummus::eSuccess)
				goto error;

			PDFRectangle rightRect=mediaBox;
			rightRect.LowerLeftX=rightRect.UpperRightX/2;
			p2->SetMediaBox(rightRect);
			status=pdfWriter2.MergePDFPagesToPage(p2,srcPath,singlePageRange);
			if (status!=PDFHummus::eSuccess)
				goto error;

			status=pdfWriter2.WritePageAndRelease(p2);
			if (status!=PDFHummus::eSuccess)
				goto error;

			status=pdfWriter2.EndPDF();
			if (status!=PDFHummus::eSuccess)
				goto error;
		}

		else{

			PDFWriter pdfWriter;
			PDFPage* page=new PDFPage();
			status=pdfWriter.StartPDF(fullDest+".pdf", ePDFVersion13);
			page->SetMediaBox(mediaBox);
			PDFPageRange singlePageRange;

			singlePageRange.mType = PDFPageRange::eRangeTypeSpecific;
			singlePageRange.mSpecificRanges.push_back(ULongAndULong(i,i));

			status=pdfWriter.MergePDFPagesToPage(page,srcPath,singlePageRange);
			if (status!=PDFHummus::eSuccess)
				goto error;

			status=pdfWriter.WritePageAndRelease(page);
			if (status!=PDFHummus::eSuccess)
				goto error;

			status=pdfWriter.EndPDF();
			if (status!=PDFHummus::eSuccess)
				goto error;

		}
		continue;
error:
		errorMsg+="failed to split page "+intZeroPadding(i+1,pageNum)+"\n";

	}

	return status;
}

//int to string then add zeroes to match the length of c
//this helps viewing files sorted by filename (so _020 is placed after _019 instead of _1)
std::string PDFCore::intZeroPadding(int i,int c){
	std::stringstream ss;
	ss<<i;
	std::string baseNum= ss.str();
	int iDigit=(int)log10(i)+1;
	int cDigit=(int)(log10(c))+1;
	while(iDigit!=cDigit){
		baseNum='0'+baseNum;
		iDigit++;
	}
	return baseNum;
}

std::string PDFCore::getErrorMsg(){
	return errorMsg;
}
