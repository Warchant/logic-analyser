#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "defines.h"
#include <QMainWindow>
#include <QMessageBox>
#include <math.h>
#include <analyser.h>

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
    void on_pushButton_not_clicked();

    void on_pushButton_and_clicked();

    void on_pushButton_or_clicked();

    void on_pushButton_xor_clicked();

    void on_pushButton_imp_clicked();

    void on_pushButton_eq_clicked();

    void on_pushButton_analyse_clicked();

private:

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
