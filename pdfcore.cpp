#include "pdfcore.h"
#include <podofo/podofo.h>
#include <sstream>
#include <cmath>
#include <iostream>

using namespace PoDoFo;

PDFCore::PDFCore(){
	errorMsg="None";
}

int PDFCore::merge(std::vector<std::string> &files, std::string dest){
	PdfMemDocument mergingFile;
	try{
		for (unsigned int i=0;i<files.size();i++){
			PdfMemDocument input(files[i].c_str());
			mergingFile.Append(input);
		}
		mergingFile.Write(dest.c_str());
		return 0;
	}
	catch(PdfError &e){
		e.PrintErrorMsg();
		errorMsg=PdfError::ErrorMessage(e.GetError());
		return e.GetError();
	}
}


int PDFCore::split(std::string srcPath,std::string destDir,std::string baseFileName){
	try{
		PdfMemDocument srcFile(srcPath.c_str());
		int count=srcFile.GetPageCount();
		std::cout<<srcPath<<std::endl;
		std::cout<<destDir<<std::endl;
		std::cout<<baseFileName<<std::endl;
		std::cout<<count<<std::endl;
		for (int i=0;i<count;i++){
			PdfMemDocument newFile;
			newFile.InsertExistingPageAt(srcFile,i,0);
			//TODO: might need to check if file exists already
			std::string fullDest=destDir+'/'+baseFileName+"_"+intZeroPadding(i+1,count)+".pdf";
			std::cout<<fullDest<<std::endl;
			newFile.Write(fullDest.c_str());
		}
		return 0;
	}
	catch(PdfError &e){
		e.PrintErrorMsg();
		errorMsg=PdfError::ErrorMessage(e.GetError());
		return e.GetError();
	}
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
