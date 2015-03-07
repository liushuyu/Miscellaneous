#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdexcept>
#include <QProcess>
#include <QByteArray>
#include <QTextBrowser>
#include <QFileDialog>
#include <QDesktopServices>
#include <QComboBox>
#include <QMessageBox>
#include <QFontDialog>
#include <QtDebug>

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
    void puzzlecmd();
    int checkpy();
    int execcmd(QString ProcName,QStringList Args);
    int choosevideo();
    int choosefont();
    int choosedanmaku();
    void cmdoutput();
    int pyoutput();
    void showaboutqt();
    void showaboutgui();
    void showaboutd2a();
    void resetform();

private:
    Ui::MainWindow *ui;
    QProcess commandProcess;
    QProcess testpy;
};

#endif // MAINWINDOW_H
