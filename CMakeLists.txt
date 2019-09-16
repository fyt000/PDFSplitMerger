cmake_minimum_required(VERSION 3.1.0)

project(PDFSplitMerger)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(PDFSplitMerger
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    pdfcore.cpp
)

add_subdirectory(PDF-Writer)
include_directories(PDF-Writer/PDFWriter)

target_link_libraries(PDFSplitMerger Qt5::Widgets PDFWriter FreeType LibJpeg LibTiff Zlib)
