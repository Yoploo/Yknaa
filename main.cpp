#include <sqlite3.h>
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include <QFileDialog>
#include <QFile>
#include <QLineEdit>
#include "Controller/user.h"

// Déclaration de la fonction ajouterDeck (assurez-vous d'ajuster les paramètres selon vos besoins)
int createLoginPage(QStackedWidget &stackedWidget, sqlite3 *db, struct user *myUser, QWidget &loginPage);
void createRegisterPage(QStackedWidget &stackedWidget, sqlite3 *db);
void createMenuBar(QMenuBar &menuBar, QStackedWidget &stackedWidget);
void createTopButtonsLayout(QHBoxLayout &topButtonsLayout, QStackedWidget &stackedWidget);
void createTextEditPage(QStackedWidget &stackedWidget, const QString &pageTitle, int buttonIndex, sqlite3 *db, int user_id);

int main(int argc, char *argv[]) {
    struct user myUser{
            -1,
            "",
            "",
            0
    };
    // Ouvrir la base de données
    sqlite3 *db;
    int result = sqlite3_open("Ykna.db", &db);
    if (result != SQLITE_OK) {
        printf("Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    QApplication app(argc, argv);

    // Create the main window
    QWidget window;
    window.setWindowTitle("Ykna");

    // Create the layout
    QVBoxLayout layout;

    // Create a menu bar
    QMenuBar menuBar;
    QStackedWidget stackedWidget;

    // Create buttons and connect them to change the current page
    createMenuBar(menuBar, stackedWidget);

    // Create the top buttons layout
    QHBoxLayout topButtonsLayout;

    // Create the login page
    QWidget loginPage;
    int loginResult = createLoginPage(stackedWidget, db, &myUser, loginPage);
    myUser.user_id = loginResult;

    // Create the register page
    createRegisterPage(stackedWidget, db);


    // Set the top buttons layout
    layout.addWidget(&menuBar);
    layout.addLayout(&topButtonsLayout);
    layout.addWidget(&stackedWidget);

    // Create the welcome label
    QLabel *welcomeLabel = new QLabel("Welcome to Ykna!");
    welcomeLabel->setAlignment(Qt::AlignCenter);
    stackedWidget.addWidget(welcomeLabel);

    // Create top buttons for the welcome page
    createTopButtonsLayout(topButtonsLayout, stackedWidget);

    // Set the layout
    window.setLayout(&layout);

    // Connect the menu actions to slots or functions for handling
    if (menuBar.actions().size() > 2) {
        QObject::connect(menuBar.actions().at(2), &QAction::triggered, &app, &QApplication::quit); // exitAction
    } else {
        qDebug() << "Exit action not found!";
    }

    // Show the window
    window.show();

    // Switch to the login page initially
    stackedWidget.setCurrentWidget(&loginPage);

    // Run the application
    return app.exec();
}
