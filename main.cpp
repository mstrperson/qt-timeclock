#include "mainwindow.h"
#include <QApplication>
#include <thread>
#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include <QtSql/QtSql>
#include <QtSql/QMYSQLDriver>
#include <QtSql/QSqlDatabase>


QString HOST;
QString UNAME;
QString PWD;

/**
 * Execute a shell command and get the command's output in a String.
 */
std::string exec(const char* cmd)
{
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if(!pipe) return "ERROR";

    char buffer[128];
    std::string result = "";

    while(!feof(pipe.get()))
    {
        if(fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }

    return result;
}

void nfcTask(MainWindow* w)
{
    // Initialize the database connection for the nfc thread.
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(HOST);
    db.setDatabaseName(UNAME);
    db.setUserName(UNAME);
    db.setPassword(PWD);

    // run this thread forever!
    while(1)
    {
		// do - while loop executes once before checking the condition.
        do
        {
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(20));
        } while(w->isLoggedIn());  // If there is a user logged in, wait 20ms.

        // check for an RFID card swipe.
        // This blocks this thread for up to 30 seconds waiting for a card swipe.
        std::string resp = exec("~/libnfc/examples/nfc-poll");


        if(strcmp(resp.c_str(), "No target found.\n") == 0 || strcmp(resp.c_str(), "") == 0)
        {
            // This happens when the nfc-poll call times out (every 30ish seconds).
            continue; // skip back to the start of the while(1) loop.
        }

        w->DisplayMessage("Card Swipe Detected.");
        //database shouldn't be open--but check just to be sure.
        if(!db.isOpen())
        {
            w->DisplayMessage("Re-establishing Database Connection.");

            bool ok = db.open();

            if(!ok)
            {	// the database failed to connect....
                w->DisplayMessage("Could Not Connect to Database");
                continue; // skip back to the beginning of while(1).
            }
        }

		// Open a new Query in the database.
        QSqlQuery q(db);
        // Look for a user in the database with the RFID corresponding to the swiped card.
        std::string qstr = "SELECT id, FirstName, LastName FROM user WHERE rfid = " + resp;
        q.exec(qstr.c_str());

        if(!q.next()) // if the query returned no results.
        {
            std::string msg = "No user with RFID: " + resp;
            w->DisplayMessage(msg.c_str());
            continue;
        }

        // get the ID and name of the person who swiped.  Log in that user.
        int id = q.value(0).toInt();
		
		
        QString name = "Hello, " + q.value(1).toString() + " " + q.value(2).toString();
        w->DisplayMessage(name);
        w->LoadUser(id);
		
		// close the database connection.
        db.close();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOverrideCursor(QCursor(Qt::BlankCursor));

    // Style the buttons so that they are visibly different when disabled
    // and make the scroll bar on the side of the text box large enough to
    // grab with the touch screen.
    a.setStyleSheet("QPushButton:!enabled { color:rgb(60, 60, 60); }"
                    "QPushButton { color:rgb(255, 255, 255); }"
                    "QScrollBar:vertical { width: 40px; background-color:lavender; }");

    bool auth = false;

    // Read the database connection config file.
	// Config file should be of the form:
	// |hostname or ip
	// |database user name
	// |database user password
	//
	// The configuration file is loaded from this address.
    QFile file("/home/pi/.mysql_auth");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        HOST=in.readLine();
        UNAME = in.readLine();
        PWD = in.readLine();

        file.close();
        auth = true;
    }

    MainWindow w(NULL, UNAME, PWD, HOST);
    w.showFullScreen();

    // if the config file was not accessible, print an error message.
    if(!auth)
    {
        w.DisplayMessage("Could not open Auth File.");
    }

    // start the nfc thread.
    std::thread nfc(nfcTask, &w);

    return a.exec();
}
