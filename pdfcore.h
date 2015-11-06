#ifndef PDFCORE_H
#define PDFCORE_H
#include <string>
#include <vector>

class PDFCore
{
public:
	PDFCore();
	int merge(std::vector<std::string>&,std::string);
	int split(std::string, std::string, std::string, bool split);
	std::string getErrorMsg();

private:
	std::string errorMsg;
	std::string intZeroPadding(int, int c);
};

#endif // PDFCORE_H
