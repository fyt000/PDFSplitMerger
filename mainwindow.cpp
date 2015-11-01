#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

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
	if (curRow){
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
