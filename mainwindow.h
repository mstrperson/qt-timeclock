#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QString u = "database name", QString p = "password", QString h = "host");
    ~MainWindow();
    void DisplayMessage(QString msg);
    void ClearMessages();
    void LoadUser(int id);
    bool isLoggedIn();
    void setCreds(QString n, QString p);

private:
    bool loggedIn;
    int userId;
    int signInCount;
    void setActionEnabled(bool enable);
    void displayUserIds();
    void displayCurrentSignIns();
    void showAllStats();
    void printHelp();
    QString host;
    QString uname;
    QString pwd;
    QSqlDatabase db;

private slots:
    void on_btn_0_clicked();

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_7_clicked();

    void on_btn_8_clicked();

    void on_btn_9_clicked();

    void on_btn_clear_clicked();

    void on_btn_accept_clicked();

    void on_btn_signin_clicked();

    void on_btn_signout_clicked();

    void on_btn_status_clicked();

    void on_btn_history_clicked();

    void updateTime();

    void idleTimeout();

private:
    Ui::MainWindow *ui;
    QTimer *idleTimer;
};

#endif // MAINWINDOW_H
