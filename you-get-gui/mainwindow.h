#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

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
    int vaildateip(QString inputip);
    void saveconfig();
    int vaildateurl(QString inputurl);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
