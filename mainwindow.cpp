#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QtSql>
#include <QtSql/QMYSQLDriver>
#include <QtSql/QSqlDatabase>
#include <QDateTime>
#include <thread>

// TimeSpan stuff.
// Used in calculations of time on the clock.
struct timespan
{
    int seconds;
    int minutes;
    int hours;
    int days;
};

/**
 * @brief addSeconds for use with calculating the time delta between
 *        two QDateTime objects for the time clock calculations.
 *        Use with QDateTime.secsTo(other) to calculate time delta.
 * @param ts - timespan to which seconds will be added
 * @param seconds - number of seconds to add to this timespan.
 */
void addSeconds(timespan &ts, int seconds)
{
    int days = seconds / (24 * 3600);
    if(days > 0)
    {
        ts.days += days;
        seconds -= days * (24*3600);
    }

    int hours = seconds / 3600;
    ts.hours += hours;
    seconds -= hours * 3600;

    int minutes = seconds / 60;
    ts.minutes += minutes;
    seconds -= minutes*60;

    ts.seconds += seconds;

    while(ts.seconds>=60)
    {
        ts.seconds -= 60;
        ts.minutes++;
    }

    while(ts.minutes>=60)
    {
        ts.minutes-=60;
        ts.hours++;
    }

    while(ts.hours >= 24)
    {
        ts.hours-=24;
        ts.days++;
    }
}

/**
 * @brief Conver the timespan to a string.  Only includes the days field if it is non zero.
 * @param ts
 * @return Human readable representation of the given timespan.
 */
QString toString(timespan ts)
{
    if(ts.days > 0)
    {
        return QString("%1 days, %2 hours, %3 minutes, %4 seconds").arg(ts.days).arg(ts.hours).arg(ts.minutes).arg(ts.seconds);
    }

    return QString("%1 hours, %2 minutes, %3 seconds").arg(ts.hours).arg(ts.minutes).arg(ts.seconds);
}

// End Timespan stuff.

/**
 * @brief MainWindow::MainWindow
 * @param parent usually NULL
 * @param u username/database for the timeclock.
 * @param p password for the user.
 * @param h mysql server host name.
 */
MainWindow::MainWindow(QWidget *parent, QString u, QString p, QString h) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->host = h;
    this->uname = u;
    this->pwd = p;

    db = QSqlDatabase::addDatabase("QMYSQL", "window_thread");
    db.setHostName(this->host);
    db.setDatabaseName(this->uname);
    db.setUserName(this->uname);
    db.setPassword(this->pwd);
    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));

    if(!ok)
    {
        DisplayMessage("Could Not Connect to Database...");
        return;
    }

    QSqlQuery query(db);
    QString qstr = QString("SELECT id, TimeIn, TimeOut FROM timesheet_entry WHERE TimeIn>CURDATE() AND TimeIn=TimeOut");
    query.exec(qstr);

    signInCount = 0;
    while(query.next())
    {
        signInCount++;
    }

    db.close();

    idleTimer = new QTimer(this);
    connect(idleTimer, SIGNAL(timeout()), this, SLOT(idleTimeout()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(5000);
    ui->setupUi(this);
    setActionEnabled(false);
    this->loggedIn = false;
}

/**
 * @brief MainWindow::idleTimeout if nothing is done for 30 seconds, clear the display.
 */
void MainWindow::idleTimeout()
{
    ClearMessages();
    ui->keypad_display->display(0);
    setActionEnabled(false);
    loggedIn = false;
    userId = -1;
    idleTimer->stop();
}

/**
 * @brief MainWindow::updateTime show the time and number of people signed in on the status bar.
 */
void MainWindow::updateTime()
{
    ui->statusBar->showMessage(QTime().currentTime().toString("hh:mm ap") + QString("\t\tThere are %1 people signed in.").arg(signInCount));
}

/**
 * @brief MainWindow::isLoggedIn
 * @return true if there is a user currently logged in.
 */
bool MainWindow::isLoggedIn()
{
    return loggedIn;
}

MainWindow::~MainWindow()
{
    delete db;
    delete ui;
}

/**
 * @brief MainWindow::LoadUser Enables the action bar and stores the logged in user info.
 * @param id user.id for the person currently accessing the terminal.
 */
void MainWindow::LoadUser(int id)
{
    setActionEnabled(true);
    loggedIn = true;
    userId = id;
    DisplayMessage("Ready.");
}

/**
 * @brief MainWindow::setActionEnabled sets the enabled field on each of the action buttons.
 * @param enable enable or disable the action buttons.
 */
void MainWindow::setActionEnabled(bool enable)
{
    ui->btn_signin->setEnabled(enable);
    ui->btn_signout->setEnabled(enable);
    ui->btn_status->setEnabled(enable);
    ui->btn_history->setEnabled(enable);
}

/**
 * @brief MainWindow::DisplayMessage displays a message in the output_display.
 *        This also triggers the idleTimer to start.
 * @param msg Message to be displayed.
 */
void MainWindow::DisplayMessage(QString msg)
{
    idleTimer->stop();
    ui->output_display->append(msg);
    idleTimer->start(30000);
}

/**
 * @brief MainWindow::ClearMessages clears the output_display.  Cannot be called from a separate thread.
 */
void MainWindow::ClearMessages()
{
    ui->output_display->clear();
}

void MainWindow::on_btn_0_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_1_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val++;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_2_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 2;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_3_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 3;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_4_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 4;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_5_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 5;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_6_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 6;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_7_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 7;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_8_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 8;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_9_clicked()
{
    int val = ui->keypad_display->intValue();
    val *= 10;
    val += 9;
    ui->keypad_display->display(val);
}

void MainWindow::on_btn_clear_clicked()
{
    ui->keypad_display->display(0);
    ui->output_display->clear();
    userId = -1;
    loggedIn = false;
    setActionEnabled(false);
}

/**
 * @brief MainWindow::displayUserIds Display the list of user.id with the corresponding names.
 */
void MainWindow::displayUserIds()
{
    ClearMessages();
    DisplayMessage("Getting User Ids");
    DisplayMessage("________________________________");

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));

    if(!ok)
    {
        DisplayMessage("Could Not Connect to Database...");
        return;
    }

    QSqlQuery query(db);
    QString qstr = "SELECT id, FirstName, LastName FROM user ORDER BY LastName";
    query.exec(qstr);

    while(query.next())
    {
        QString line = QString("%1 -- %2 %3").arg(query.value(0).toInt()).arg(query.value(1).toString()).arg(query.value(2).toString());
        DisplayMessage(line);
    }

    db.close();
}

/**
 * @brief MainWindow::displayCurrentSignIns Show who is currently signed in and how long they have been signed in.
 */
void MainWindow::displayCurrentSignIns()
{
    ClearMessages();
    DisplayMessage("Currently Signed In:");
    DisplayMessage("________________________________");

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));

    if(!ok)
    {
        DisplayMessage("Could Not Connect to Database...");
        return;
    }

    QSqlQuery query(db);
    QString qstr = QString("SELECT a.FirstName, a.LastName, b.TimeIn FROM user a, timesheet_entry b ") +
                   QString("WHERE a.id=b.userId AND b.TimeIn>CURDATE() AND b.TimeIn=b.TimeOut");
    query.exec(qstr);

    while(query.next())
    {
        QDateTime ti = query.value(2).toDateTime();
        timespan ts;
        ts.days=0;
        ts.hours=0;
        ts.minutes=0;
        ts.seconds=0;
        addSeconds(ts, ti.secsTo(ti.currentDateTime()));
        QString line = QString("%1 %2 -- %3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(toString(ts));
        DisplayMessage(line);
    }

    db.close();
}

/**
 * @brief MainWindow::showAllStats shows time on the clock for all users.
 */
void MainWindow::showAllStats()
{
    ClearMessages();
    DisplayMessage("Stats for all Users:");
    DisplayMessage("________________________________");

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));

    if(!ok)
    {
        DisplayMessage("Could Not Connect to Database...");
        return;
    }

    QSqlQuery query(db);
    QString qstr = "SELECT id, FirstName, LastName FROM user ORDER BY LastName";
    query.exec(qstr);

    while(query.next())
    {
        int usrid = query.value(0).toInt();
        QString fname = query.value(1).toString();
        QString lname = query.value(2).toString();

        DisplayMessage(fname + " " + lname);
        qstr = QString("SELECT TimeIn, TimeOut FROM timesheet_entry WHERE userId=%1").arg(usrid);
        QSqlQuery q2(db);
        q2.exec(qstr);
        timespan timeOn;
        timeOn.seconds=0;
        timeOn.minutes=0;
        timeOn.hours=0;
        timeOn.days=0;
        int notSignedOutCount = 0;

        while(q2.next())
        {
            QDateTime ti = q2.value(0).toDateTime();
            QDateTime to = q2.value(1).toDateTime();
            if (to <= ti)
            {
                notSignedOutCount++;
                continue;
            }

            addSeconds(timeOn, ti.secsTo(to));
        }

        DisplayMessage(QString("%1 (%2)").arg(toString(timeOn)).arg(notSignedOutCount));
        DisplayMessage("__________________________");
    }


    db.close();
}


/**
 * @brief MainWindow::printHelp displays the list of special comands.
 */
void MainWindow::printHelp()
{
    ClearMessages();
    DisplayMessage("Enter your User ID or Swipe your card to sign in.");
    DisplayMessage("If the screen is cluttered, press 'Clear.'\n");
    DisplayMessage("Special Commands:");
    DisplayMessage("0:\tDisplay this message.");
    DisplayMessage("1111:\tPrint the list of User IDs.");
    DisplayMessage("1234:\tShow who is currently Signed In.");
    DisplayMessage("555:\tDisplay Signin Totals for all Users.");
}

/**
 * @brief MainWindow::on_btn_accept_clicked
 *        Execute a special command or sign in from the keypad.
 */
void MainWindow::on_btn_accept_clicked()
{
    // Special Commands!

    if(ui->keypad_display->intValue()==0)
    {
        printHelp();
        return;
    }

    if(ui->keypad_display->intValue()==1111)
    {
        displayUserIds();
        ui->keypad_display->display(0);
        return;
    }

    if(ui->keypad_display->intValue()==1234)
    {
        displayCurrentSignIns();
        ui->keypad_display->display(0);
        return;
    }

    if(ui->keypad_display->intValue()==555)
    {
        showAllStats();
        ui->keypad_display->display(0);
        return;
    }

    // End Special Commands

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
    if(!ok)
    {
        DisplayMessage("Could Not Connect to Database...");
        return;
    }

    int usrid = ui->keypad_display->intValue();
    QString idstr = QString::number(usrid);

    QSqlQuery q(db);
    QString qstr = QString("SELECT FirstName, LastName FROM user WHERE id = ") + idstr;
    q.exec(qstr);
    if(!q.next())
    {
        QString msg = QString("Invalid Id: ") + idstr;
        DisplayMessage(msg);
        DisplayMessage("Type 1111 to see the list of ID numbers.");
        return;
    }

    QString name = "Hello, " + q.value(0).toString() + " " + q.value(1).toString();
    DisplayMessage(name);
    LoadUser(usrid);
    db.close();
}

/**
 * @brief MainWindow::on_btn_signin_clicked
 *          Clock In.  If the user is already clocked in, print an appropriate message.
 */
void MainWindow::on_btn_signin_clicked()
{

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
    if(!ok)
    {
        ui->output_display->append("Failed to connect to database...");
        return;
    }

    QString idstr = QString::number(userId);

    QSqlQuery query(db);
    QString qstr = QString("SELECT id, TimeIn, TimeOut FROM timesheet_entry WHERE userId=") + idstr + QString(" AND TimeIn>CURDATE() AND TimeIn=TimeOut");
    query.exec(qstr);

    if(!query.next())
    {
        qstr = QString("INSERT INTO timesheet_entry (TimeIn, TimeOut, userId) VALUES (NOW(), NOW(), ") + idstr + QString(")");
        query.exec(qstr);
        loggedIn = false;
        userId = -1;
        setActionEnabled(false);
        ui->keypad_display->display(0);
        DisplayMessage("Successfully Signed in at:");
        DisplayMessage(QDateTime().currentDateTime().toString());
        signInCount++;
    }
    else
    {
       DisplayMessage("You are already signed in!");
    }

    db.close();

}

/**
 * @brief MainWindow::on_btn_signout_clicked
 *          Clock out.  If the user is not signed in already, print an appropriate error.
 */
void MainWindow::on_btn_signout_clicked()
{

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
    if(!ok)
    {
        ui->output_display->append("Failed to connect to database...");
        return;
    }

    QString idstr = QString::number(userId);

    QSqlQuery query(db);
    QString qstr = QString("SELECT id, TimeIn, TimeOut FROM timesheet_entry WHERE userId=") + idstr + QString(" AND TimeIn>CURDATE() AND TimeIn=TimeOut");
    query.exec(qstr);

    int teid = -1;

    if(!query.next())
    {
        DisplayMessage("You are not currently signed in.");
    }
    else
    {
        teid = query.value(0).toInt();
        qstr = "UPDATE timesheet_entry SET TimeOut=NOW() WHERE id=" + QString::number(teid);
        query.exec(qstr);
        loggedIn = false;
        userId = -1;
        setActionEnabled(false);
        ui->keypad_display->display(0);
        DisplayMessage("Sucessfully Signed Out.");
        signInCount--;
    }
    db.close();
}

/**
 * @brief MainWindow::on_btn_status_clicked
 *          Display if the user is currently signed in or not.  If so, how long have they been signed in?
 */
void MainWindow::on_btn_status_clicked()
{

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
    if(!ok)
    {
        ui->output_display->append("Failed to connect to database...");
        return;
    }

    QString idstr = QString::number(userId);

    QSqlQuery query(db);
    QString qstr = QString("SELECT id, TimeIn, TimeOut FROM timesheet_entry WHERE userId=") + idstr + QString(" AND TimeIn>CURDATE() AND TimeIn=TimeOut");
    query.exec(qstr);

    if(!query.next())
    {
        DisplayMessage("You are currently signed out.");
    }
    else
    {
        QDateTime dt = query.value(1).toDateTime();
        int seconds = dt.secsTo(dt.currentDateTime());
        int hours = seconds / 3600;
        seconds -= hours * 3600;
        int minutes = seconds / 60;

        DisplayMessage(QString("You have been signed in for %1 hours, %2 minutes.").arg(hours).arg(minutes));
    }

    db.close();
}

/**
 * @brief MainWindow::on_btn_history_clicked
 *          Calculates the total time the user has spent on the clock.
 */
void MainWindow::on_btn_history_clicked()
{

    bool ok = db.open();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
    if(!ok)
    {
        ui->output_display->append("Failed to connect to database...");
        return;
    }

    QString idstr = QString::number(userId);

    QSqlQuery query(db);
    QString qstr = QString("SELECT id, TimeIn, TimeOut FROM timesheet_entry WHERE userId=") + idstr;
    query.exec(qstr);

    timespan timeOn;
    timeOn.seconds=0;
    timeOn.minutes=0;
    timeOn.hours=0;
    timeOn.days=0;
    int notSignedOutCount = 0;

    while(query.next())
    {
        QDateTime ti = query.value(1).toDateTime();
        QDateTime to = query.value(2).toDateTime();
        if (to <= ti)
        {
            notSignedOutCount++;
            continue;
        }

        addSeconds(timeOn, ti.secsTo(to));
    }

    DisplayMessage("You have been signed on for:");
    DisplayMessage(toString(timeOn));
    DisplayMessage(QString("You have forgotten to sign out %1 times...").arg(notSignedOutCount));

    db.close();
}
