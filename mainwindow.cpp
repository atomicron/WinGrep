#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , grep(new QProcess())
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("WinGrep");
    ui->list->setHorizontalHeaderLabels(QStringList() << "File name" << "ext" << "Directory");
    connect (grep, SIGNAL(readyReadStandardOutput()), this, SLOT(read_line()));
    connect (grep, SIGNAL(finished(int , QProcess::ExitStatus)), this, SLOT(set_status_finished()));
    connect (ui->list, SIGNAL(itemActivated(QTableWidgetItem *)), this, SLOT(item_activated(QTableWidgetItem*)));
    connect (ui->list, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(item_activated(QTableWidgetItem*)));
}

MainWindow::~MainWindow()
{
    grep->close();
    delete ui;
}

void MainWindow::item_activated(QTableWidgetItem *item)
{
    switch (item->column()) {
    case 0:
    {
        int row = item->tableWidget()->currentRow();
        QString path = item->tableWidget()->item(row, 2)->text();
        QString suffix = item->tableWidget()->item(row, 1)->text();
        if (!suffix.isEmpty()) {
            QDesktopServices::openUrl(QUrl(path + "/" + item->text() + "." + suffix ));
        } else {
            QDesktopServices::openUrl(QUrl(path + "/" + item->text() + "." + suffix ));
        }
        break;
    }
    case 1:
        break;
    case 2:
        QDesktopServices::openUrl(QUrl(item->text()));
        break;
    default:
        break;
    }
}

void MainWindow::read_line()
{
    while (grep->canReadLine())
        read_results(grep->readLine());
}

void MainWindow::read_results(QByteArray byte_array)
{
    QString line = byte_array;
    line.remove("\n");

    QFileInfo file(line);
    int row = ui->list->rowCount();
    ui->list->insertRow(row);
    ui->list->setItem(row, 0, new QTableWidgetItem(file.baseName()));
    ui->list->setItem(row, 1, new QTableWidgetItem(file.suffix()));
    ui->list->setItem(row, 2, new QTableWidgetItem(file.absolutePath()));
}

void MainWindow::search(QString pattern, QString path)
{
    grep->close();
    for (int i=0; i<ui->list->rowCount(); ++i)
        ui->list->removeRow(i);

    set_status(SEARCHING);

    QString program = "grep/grep.exe";
    QStringList arguments;
    arguments << "-ril" << pattern << path;

    qDebug() << "Arguments: ";
    for (auto x : arguments)
        qDebug() << x;

    grep->start(program, arguments);
}

void MainWindow::on_btn_search_clicked()
{
    search_pattern = ui->input_pattern->text();
    search_path = ui->input_directory->text();

    search(search_pattern, search_path);
}

void MainWindow::on_btn_browse_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.exec();

    QString new_path = dialog.directory().path();
    ui->input_directory->setText(new_path);
    search_path = new_path;
}

void MainWindow::on_btn_stop_clicked()
{
    grep->close();
    set_status(FINISHED);
}

void MainWindow::on_input_pattern_returnPressed()
{
    on_btn_search_clicked();
}

void MainWindow::set_status_finished()
{
   set_status(FINISHED);
}

void MainWindow::set_status(STATUSES status)
{
    switch (status) {
    case FINISHED:
        ui->status->setText("Finished");
        break;
    case NOT_FOUND:
        ui->status->setText("Nothing found");
        break;
    case WAITING:
        ui->status->setText("Waiting for input");
        break;
    case SEARCHING:
        ui->status->setText("Searching..");
        break;
    default:
        break;
    }
}


void MainWindow::on_btn_exit_clicked()
{
    grep->close();
    QApplication::exit(0);
}
