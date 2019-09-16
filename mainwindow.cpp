#include "mainwindow.h"
#include "pdfcore.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow} {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_mergeBrowse_clicked() {
    auto targetDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),
                                                       QFileDialog::ShowDirsOnly);
    ui->mergeDestDirEdit->setText(targetDir);
}

void MainWindow::on_mergeAddFile_clicked() {
    auto fileNames = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::homePath(),
                                                   tr("PDF Files (*.pdf)"));
    ui->mergeFileListWidget->addItems(fileNames);
}

void MainWindow::on_mergeDeleteFile_clicked() {
    auto item = ui->mergeFileListWidget->currentItem();
    delete item;
}

void MainWindow::on_mergeMoveUp_clicked() {
    if (ui->mergeFileListWidget->currentItem() == nullptr) {
        return;
    }
    auto curRow = ui->mergeFileListWidget->currentIndex().row();
    if (curRow != 0) { // not the first item
        auto *prevItem = ui->mergeFileListWidget->takeItem(curRow - 1);
        ui->mergeFileListWidget->insertItem(curRow, prevItem);
    }
}

void MainWindow::on_mergeMoveDown_clicked() {
    if (ui->mergeFileListWidget->currentItem() == nullptr) {
        return;
    }
    auto curRow = ui->mergeFileListWidget->currentIndex().row();
    if (curRow != ui->mergeFileListWidget->count() - 1) {
        auto *prevItem = ui->mergeFileListWidget->takeItem(curRow + 1);
        ui->mergeFileListWidget->insertItem(curRow, prevItem);
    }
}

void MainWindow::on_splitBrowseFile_clicked() {
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(),
                                                 tr("PDF Files (*.pdf)"));
    ui->splitFileEdit->setText(fileName);
}

void MainWindow::on_splitBrowseDir_clicked() {
    auto targetDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), QDir::homePath(),
                                                       QFileDialog::ShowDirsOnly);
    ui->splitDirEdit->setText(targetDir);
}

void MainWindow::warningMsgBox(const QString &msg, bool isWarning) {
    QMessageBox msgBox{};
    msgBox.setText(msg);
    if (isWarning) {
        msgBox.setIcon(QMessageBox::Warning);
    }
    msgBox.exec();
}

void MainWindow::on_mergePush_clicked() {
    if (ui->mergeDestDirEdit->text().trimmed().length() == 0 ||
        ui->mergeDestFileEdit->text().trimmed().length() == 0) {
        warningMsgBox(tr("Missing destination directory or filename."));
        return;
    }

    if (ui->mergeFileListWidget->count() <= 1) {
        warningMsgBox(tr("No files to merge."));
        return;
    }

    if (!QDir(ui->mergeDestDirEdit->text()).exists()) {
        QDir().mkdir(ui->mergeDestDirEdit->text());
    }

    auto finalPath = QDir(ui->mergeDestDirEdit->text()).filePath(ui->mergeDestFileEdit->text());
    std::vector<std::string> fileNames;
    for (int i = 0; i < ui->mergeFileListWidget->count(); i++) {
        fileNames.push_back(ui->mergeFileListWidget->item(i)->text().toStdString());
    }

    std::string errMsg{};
    if (!PDFCore::merge(errMsg, fileNames, finalPath.toStdString())) {
        warningMsgBox(tr(errMsg.c_str()));
    } else {
        warningMsgBox(tr("Files have been successfully merged"), false);
    }
}

void MainWindow::on_splitPush_clicked() {
    auto &srcFileStr = ui->splitFileEdit->text();
    auto &destPathStr = ui->splitDirEdit->text();
    if (destPathStr.trimmed().length() == 0 || srcFileStr.trimmed().length() == 0) {
        warningMsgBox(tr("Missing source file or destination directory."));
        return;
    }
    QFile srcFile{srcFileStr};
    if (!srcFile.exists()) {
        warningMsgBox(tr("Invalid source file."));
        return;
    }
    if (!QDir(destPathStr).exists()) {
        QDir().mkdir(destPathStr);
    }

    QFileInfo fileInfo{srcFile};
    std::string errMsg{};
    auto baseFileName = fileInfo.baseName().toStdString();
    if (!PDFCore::split(errMsg, srcFileStr.toStdString(), destPathStr.toStdString(), baseFileName,
                        ui->splitHalfCheckBox->isChecked())) {
        warningMsgBox(tr(errMsg.c_str()));
    } else {
        warningMsgBox(tr("File have been splitted successfully"), false);
    }
}
