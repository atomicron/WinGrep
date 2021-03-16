#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum STATUSES {
        FINISHED,
        NOT_FOUND,
        WAITING,
        SEARCHING
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString search_path;
    QString search_pattern;
    QStringList results;
    QProcess* grep;

private slots:
    void on_btn_search_clicked();
    void on_btn_browse_clicked();
    void on_btn_stop_clicked();
    void on_input_pattern_returnPressed();
    void set_status_finished();
    void set_status(STATUSES);
    void read_line();
    void item_activated(QTableWidgetItem*);

    void on_btn_exit_clicked();

private:
    Ui::MainWindow *ui;

    void search(QString pattern, QString path);
    void read_results(QByteArray byte_array);
};
#endif // MAINWINDOW_H
