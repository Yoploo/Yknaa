#include "View/front.h"
#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include <QTextEdit>
#include <QFileDialog>


int main(int argc, char *argv[]) {
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

    // Create pages for each button
    for (int i = 1; i <= 5; ++i) {
        createTextEditPage(stackedWidget, "Page for Button " + QString::number(i), i - 1);
    }

    // Create the login page
    createLoginPage(stackedWidget);

    // Create the register page
    createRegisterPage(stackedWidget);

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

    // Switch to the welcome page initially
    stackedWidget.setCurrentIndex(5);

    // Run the application
    return app.exec();
}
