#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include "pdfcore.h"
#include <vector>
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_mergeBrowse_clicked(){
	QString targetDir=QFileDialog::getExistingDirectory(this,tr("Open Directory"),QDir::homePath(),QFileDialog::ShowDirsOnly);
	ui->mergeDestDirEdit->setText(targetDir);
}

void MainWindow::on_mergeAddFile_clicked(){
	QStringList fileNames=QFileDialog::getOpenFileNames(this,tr("Open File"),QDir::homePath(),tr("PDF Files (*.pdf)"));
	ui->mergeFileListWidget->addItems(fileNames);
}

void MainWindow::on_mergeDeleteFile_clicked(){
	QListWidgetItem *item=ui->mergeFileListWidget->currentItem();
	delete item;
}

void MainWindow::on_mergeMoveUp_clicked(){
	if (ui->mergeFileListWidget->currentItem()==NULL)
		return;
	int curRow=ui->mergeFileListWidget->currentIndex().row();
	if (curRow){ //not the first item
		QListWidgetItem* prevItem=ui->mergeFileListWidget->takeItem(curRow-1);
		ui->mergeFileListWidget->insertItem(curRow,prevItem);
	}
}

void MainWindow::on_mergeMoveDown_clicked(){
	if (ui->mergeFileListWidget->currentItem()==NULL)
		return;
	int curRow=ui->mergeFileListWidget->currentIndex().row();
	if (curRow!=ui->mergeFileListWidget->count()-1){
		QListWidgetItem* prevItem=ui->mergeFileListWidget->takeItem(curRow+1);
		ui->mergeFileListWidget->insertItem(curRow,prevItem);
	}
}

void MainWindow::on_splitBrowseFile_clicked(){
	QString fileName=QFileDialog::getOpenFileName(this,tr("Open File"),QDir::homePath(),tr("PDF Files (*.pdf)"));
	ui->splitFileEdit->setText(fileName);
}

void MainWindow::on_splitBrowseDir_clicked(){
	QString targetDir=QFileDialog::getExistingDirectory(this,tr("Open Directory"),QDir::homePath(),QFileDialog::ShowDirsOnly);
	ui->splitDirEdit->setText(targetDir);
}

void MainWindow::warningMsgBox(QString q,bool isWarning){
	QMessageBox msgBox;
	msgBox.setText(q);
	if (isWarning)
		msgBox.setIcon(QMessageBox::Warning);
	msgBox.exec();
}

void MainWindow::on_mergePush_clicked()
{

	if (ui->mergeDestDirEdit->text().trimmed().length()==0||ui->mergeDestFileEdit->text().trimmed().length()==0){
		warningMsgBox(tr("Missing destination directory or filename."));
		return;
	}
	if (ui->mergeFileListWidget->count()<=1){
		warningMsgBox(tr("No files to merge."));
		return;
	}

	if (!QDir(ui->mergeDestDirEdit->text()).exists()){
		QDir().mkdir(ui->mergeDestDirEdit->text());
	}
	QString finalPath=QDir(ui->mergeDestDirEdit->text()).filePath(ui->mergeDestFileEdit->text());
	std::vector<std::string> fileNames;
	for (int i=0;i<ui->mergeFileListWidget->count();i++)
		fileNames.push_back(ui->mergeFileListWidget->item(i)->text().toStdString());
	//merge the files, potential problem: source files got deleted at this point
	int status=p.merge(fileNames,finalPath.toStdString());
	std::cout<<status<<std::endl;
	if (status)
		warningMsgBox(tr(p.getErrorMsg().c_str()));
	else
		warningMsgBox(tr("Files have been successfully merged"),false);
}

void MainWindow::on_splitPush_clicked(){
	QString srcFileStr(ui->splitFileEdit->text());
	QString destPathStr(ui->splitDirEdit->text());
	if (destPathStr.trimmed().length()==0||srcFileStr.trimmed().length()==0){
		warningMsgBox(tr("Missing source file or destination directory."));
		return;
	}
	QFile srcFile(srcFileStr);
	if (!srcFile.exists()){
		warningMsgBox(tr("Invalid source file."));
		return;
	}
	if (!QDir(destPathStr).exists()){
		QDir().mkdir(destPathStr);
	}

	QFileInfo fileInfo(srcFile);
	std::string baseFileName(fileInfo.baseName().toStdString());
	int status=p.split(srcFileStr.toStdString(),destPathStr.toStdString(),baseFileName,ui->splitHalfCheckBox->isChecked());
	std::cout<<status<<std::endl;
	if (status)
		warningMsgBox(tr(p.getErrorMsg().c_str()));
	else
		warningMsgBox(tr("File have been splitted successfully"),false);
}
