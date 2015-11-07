# PDFSplitMerger
Split and merge pdf files. Three functionalities: merge multiple pdfs into one, split pdf into pages and an option to cut each page in half (useful for fixing scanned files).

Uses podofo for the pdf library and Qt as the GUI. 

podofo 0.9.3
http://podofo.sourceforge.net/

Qt 5.5
http://www.qt.io/qt-framework/

Podofo makes it difficult to only copy the required resources/objects when splitting a pdf. I will be moving to hummus pdf.

# Linux
Please compile podofo and fix the lib path and header path in the .pro file. Run qmake and make or compile with QtCreator.
Also check bin for compiled binaries/libraries on Ubuntu 13.04.

# Windows
I could not get podofo to compile and function correctly on windows. Mingw compiled library does not pass all the given tests. Compiling on visual studio seems way too tedious.

