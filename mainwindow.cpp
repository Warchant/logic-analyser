#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_not_clicked()
{
    ui->plainTextEdit->insertPlainText(NOT);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_pushButton_and_clicked()
{
    ui->plainTextEdit->insertPlainText(AND);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_pushButton_or_clicked()
{
    ui->plainTextEdit->insertPlainText(OR);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_pushButton_xor_clicked()
{
    ui->plainTextEdit->insertPlainText(XOR);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_pushButton_imp_clicked()
{
    ui->plainTextEdit->insertPlainText(IMP);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_pushButton_eq_clicked()
{
    ui->plainTextEdit->insertPlainText(EQ);
    ui->plainTextEdit->setFocus();
}

void MainWindow::on_pushButton_analyse_clicked()
{
    QString expression = ui->plainTextEdit->toPlainText();
    Analyser *A = new Analyser(expression);
    A->setPb(ui->progressBar);

    try
    {
        A->run();
    }
    catch (const QString &msg)
    {
        ui->progressBar->setValue(ui->progressBar->maximum());
        QMessageBox::information(NULL, " ", msg);
    }
    catch (...)
    {
        QMessageBox::information(NULL, "Error", "Something bad was happened... Unexpected exception.");
    }
    ui->progressBar->hide();
    delete A;
}

