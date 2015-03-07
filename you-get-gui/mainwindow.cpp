#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(saveconfig()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
int MainWindow::vaildateip(QString inputip)
{
    QRegExp testip("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    int pos = testip.indexIn(inputip);
    if (pos <0 ) {
        return 1;
    }else{
        return 0;
    }
}

void MainWindow::saveconfig(){
    if ((vaildateip(ui->lineEdit_4->placeholderText())) && vaildateurl(ui->lineEdit_4->placeholderText())){
        QMessageBox::warning(this,tr("Wrong Address!"),tr("You have input wrong proxy address!"));
        return;
    }
}

int MainWindow::vaildateurl(QString inputurl){
    QRegExp testurl("\\bhttp:\\\\\\.*\\b\\.\\b");
    int pos = testurl.indexIn(inputurl);
    if (pos <0 ) {
        return 1;
    }else{
        return 0;
    }
}
