#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pdfcore.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_mergeBrowse_clicked();


	void on_mergeAddFile_clicked();

	void on_mergeDeleteFile_clicked();

	void on_mergeMoveUp_clicked();

	void on_mergeMoveDown_clicked();

	void on_splitBrowseFile_clicked();

	void on_splitBrowseDir_clicked();

	void on_mergePush_clicked();

	void on_splitPush_clicked();

private:
	Ui::MainWindow *ui;
	PDFCore p;
	void warningMsgBox(QString, bool isWarning=true);
};

#endif // MAINWINDOW_H
