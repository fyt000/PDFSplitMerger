#ifndef PDFCORE_H
#define PDFCORE_H
#include <EStatusCode.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace PDFCore {
bool merge(std::string &errMsg, const std::vector<std::string> &files, const std::string &dest);
bool split(std::string &errMsg, const std::string &srcPath, const std::string &destDir,
           const std::string &baseFileName, bool splitInMiddle);

class PDFException : public std::runtime_error {
public:
    PDFException(const std::string &msg) : std::runtime_error{msg} {}
};

template<typename Func>
void ExWrapper(Func &&f, const std::string &msg) {
    auto status = f();
    if (status != PDFHummus::eSuccess) {
        throw PDFException(msg);
    }
}

template<typename Func>
bool BoolWrapper(Func &&f, const std::string &msg, std::string &err) {
    auto status = f();
    if (status != PDFHummus::eSuccess) {
        err = msg;
        return false;
    }
    return true;
}

}; // namespace PDFCore

#endif // PDFCORE_H
