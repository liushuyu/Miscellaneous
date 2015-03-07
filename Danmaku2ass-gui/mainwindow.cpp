#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(puzzlecmd()));
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(choosedanmaku()));
    connect(ui->toolButton_2,SIGNAL(clicked()),this,SLOT(choosevideo()));
    connect(ui->toolButton_3,SIGNAL(clicked()),this,SLOT(choosefont()));
    connect (&commandProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(cmdoutput()));
    connect (&commandProcess,SIGNAL(readyReadStandardError()),this,SLOT(cmdoutput()));
    connect (&testpy,SIGNAL(readyReadStandardOutput()),this,SLOT(pyoutput()));
    connect (&testpy,SIGNAL(readyReadStandardError()),this,SLOT(pyoutput()));
    connect (&testpy,SIGNAL(error(QProcess::ProcessError)),this,SLOT(pyoutput()));
    connect (ui->actionAbout_Qt,SIGNAL(triggered()),this,SLOT(showaboutqt()));
    connect (ui->actionAbout_this_GUI,SIGNAL(triggered()),this,SLOT(showaboutgui()));
    connect (ui->actionAbout_Danmaku2Ass,SIGNAL(triggered()),this,SLOT(showaboutd2a()));
    connect (ui->pushButton_2,SIGNAL(clicked()),this,SLOT(resetform()));

    //connect (&commandProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT());
    checkpy();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::puzzlecmd()
{
    QStringList args;
    if (ui->danamkufn->placeholderText()==""){
        QMessageBox::warning (this,tr("Danmaku Comment File Missing"),tr("Danmaku comment file hasn't been specified!"));
        return;
    }
    if (ui->videofn->placeholderText()==""){
        QMessageBox::warning (this,tr("Video File Missing"),tr("Video file hasn't been specified!"));
        return;
    }
    if (ui->fontname->placeholderText()==""){
        QMessageBox::warning(this,tr("Font Name"),tr("Font name is not set! Falling back to Helvetica"));
        ui->fontname->setPlaceholderText("Helvetica,26");
    }
    QString saveass = QFileDialog::getSaveFileName(this,tr("Select where to save converted ass"),ui->danamkufn->placeholderText()
                                                   ,tr("ASS subtitle file(*.ass)"));
    if (saveass==""){
        QMessageBox::warning(this,tr("Can't save file"),tr("Destination not selected!"));
        saveass="";
        puzzlecmd();
    }
    QString path;
    QDir dir;
    path=dir.currentPath() +"/danmaku2ass.py";
    qDebug() << path;
        args<<path;args<<"-o";
        args<<saveass;
        args<<"-s";
        QString vheight = QString::number(ui->spinBox_5->value(),'f',0);
        QString vwidth = QString::number(ui->spinBox_6->value(),'f',0);
        qDebug() << vheight + "x" + vwidth;
        args << vheight + "x" + vwidth;
        QStringList parts = (ui->fontname->placeholderText()).split(",");
        QString plainfontname = parts.at(0);
        QString fontsize = parts.at(1);
        args<<"-fn"; args<<plainfontname;
        args<<"-fs"; args<<fontsize;
        QString alphav = QString::number((ui->spinBox_4->value())/100.0,'f',2);
        args<<"-a";args<<alphav;
        args<<"-dm";args<<QString::number(ui->spinBox->value(),'f',0);
        args<<"-ds";args<<QString::number(ui->spinBox_2->value(),'f',0);
        if (ui->spinBox_3->value()==0) {
            args<<"-p";args<<QString::number(ui->spinBox_3->value(),'f',0);
        }
        if (ui->checkBox->checkState()){
            args<<"-r";
        }
        args<<ui->danamkufn->placeholderText();
    qDebug() << "cmd is:" << "python " << args;
    checkpy();
    execcmd("python",args);
}
MainWindow::checkpy()
{
    QString pyname = "python";
    QStringList pytestargs;
    pytestargs << "-V";
    testpy.start(pyname,pytestargs);
    return 0;
}
int MainWindow::execcmd(QString ProcName, QStringList Args)
{
    commandProcess.start(ProcName, Args);
    if (!commandProcess.exitCode()) {
        return -1;
    }else{
       return 0;
    }
}
MainWindow::choosedanmaku()
{
    QString danmaku = QFileDialog::getOpenFileName(this,tr("Select Danmaku comment file"),
                                                  "",tr("Danmaku XML file(*.xml);;Danmaku JSON file(*.json)"));
    ui->danamkufn->setPlaceholderText(danmaku);
    return 0;
}
MainWindow::choosevideo()
{
    QString file = QFileDialog::getOpenFileName (this,tr("Select Video file"),"",tr("FLV MP4 video file(*.flv;*.mp4;*.f4v)"));
    ui->videofn->setPlaceholderText(file);
    return 0;
}
MainWindow::choosefont()
{
    bool ok;
    QFont typefont = QFontDialog::getFont(&ok,QFont("Helvetica", 26),this);
    if (ok){
        QString fname = (typefont.toString());
        ui->fontname->setPlaceholderText(fname);
        qDebug()<<"font"<<fname;
        return 0;
    }else{
        QMessageBox::warning(this,tr("Font selection"),tr("No font seclected!"));
        //font.clear();
        return 1;
    }
    return 0;
}
void MainWindow::cmdoutput()
{
    QByteArray cmdoutput = commandProcess.readAllStandardOutput();
    QString txtoutput = cmdoutput;
    if ((txtoutput.contains("Err"))||(txtoutput.contains("Invalid")) ||(txtoutput.contains("无效"))||(txtoutput.contains("無效"))||(txtoutput.contains("無効"))){
        ui->textEdit->append("<span style=\"color:red;\">"+txtoutput+"</span>");
    }else if ((txtoutput.contains("Invalid"))||(txtoutput.contains("未知"))){
        ui->textEdit->append("<span style=\"color:yellow;\">"+txtoutput+"</span>");
    }else{
        ui->textEdit->append(txtoutput);
    }
    cmdoutput = commandProcess.readAllStandardError();
    txtoutput = cmdoutput;
    if ((txtoutput.contains("Err"))||(txtoutput.contains("Invalid")) ||(txtoutput.contains("无效"))||(txtoutput.contains("無效"))||(txtoutput.contains("無効"))){
        ui->textEdit->append("<span style=\"color:red;\">"+txtoutput+"</span>\n");
    }else if ((txtoutput.contains("Invalid"))||(txtoutput.contains("未知"))){
        ui->textEdit->append("<span style=\"color:yellow;\">"+txtoutput+"</span>\n");
    }else{
        ui->textEdit->append(txtoutput);
    }
}
int MainWindow::pyoutput()
{
    QByteArray cmdoutput = testpy.readAllStandardOutput();
    QString txtoutput = cmdoutput;
    bool haspy;
 //   qDebug() <<txtoutput;
    if (txtoutput.contains("Python 3",Qt::CaseSensitive)){
        haspy=1;
    }
//    qDebug()<<haspy;
    if (!haspy ) {
        QMessageBox::warning(this,tr("Error"),tr("Python Error! Please check if you have installed Python"));
        return 1;
    }
    return 0;
}
void MainWindow::showaboutqt()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}
void MainWindow::showaboutgui()
{
    QMessageBox::about(this,tr("About This GUI"),tr("This GUI has been written with Qt by @liushuyu"));
}
void MainWindow::showaboutd2a()
{
    QMessageBox::about(this,tr("About Danmaku2Ass"),tr("This tool has been written by <a href=\"https://github.com/m13253/\">@Star Brilliant</a> <br>\n\
 <a href=\"https://github.com/m13253/danmaku2ass\">Danmaku2ASS</a> \
can convert comments from Niconico/Acfun/Bilibili to ASS format so that you can play it with any media player supporting ASS subtitle."));
}
void MainWindow::resetform()
{
    ui->checkBox->setChecked(0);
    ui->spinBox->setValue(5);
    ui->spinBox_2->setValue(5);
    ui->spinBox_3->setValue(0);
    ui->spinBox_4->setValue(80);
    ui->spinBox_5->setValue(1);
    ui->spinBox_6->setValue(1);
    ui->danamkufn->clear();
    ui->videofn->clear();
    ui->fontname->clear();
}
