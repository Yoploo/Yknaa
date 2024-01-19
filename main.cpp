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

/*
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
*/

#include <QApplication>
#include <QPushButton>
#include "Database/createDatabase.h"
#include <sqlite3.h>

// Déclaration de la fonction ajouterDeck (assurez-vous d'ajuster les paramètres selon vos besoins)
int addDeck(sqlite3* db, const char* subject, const char* description, const char* tag, const char* status, int user_id);
int deleteDeck(sqlite3* db, int deck_id, int user_id);
int addCard(sqlite3* db, const char* recto, const char* verso, int rank, int points, int deck_id, int user_id);
int deleteCard(sqlite3* db, int deck_id, int user_id);

int main(int argc, char **argv)
{
    // Ouvrir la base de données
    sqlite3* db;
    int result = sqlite3_open("Ykna.db", &db);
    if (result != SQLITE_OK) {
        printf("Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Créer et delete deck
    //result = addDeck(db, "Kiki", "Révision kiki", "k", "0", 3); // 1 est l'ID de l'utilisateur
    //result = deleteDeck(db, 5, 3);

    //ajouter et delete carte
    //result = addCard(db,"Hello", "Bonjour", 0, 10, 1, 1);
    //result = addCard(db,"ByeBye", "Au revoir", 0, 10, 4, 2);

    //result = deleteCard(db, 2, 1);

    // Fermer la base de données
    sqlite3_close(db);

    if (result != 0) {
        printf("Erreur lors de l'ajout du deck.\n");
        return 1;
    }

    return 0;
}