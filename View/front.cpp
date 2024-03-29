//
// Created by loloy on 18/01/2024.
//


#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStackedWidget>
#include <QTextEdit>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QLineEdit>
#include <sqlite3.h>
#include "../Controller/deck.h"
#include "../Controller/deck.c"
#include "../Controller/card.h"

void createMenuBar(QMenuBar &menuBar, QStackedWidget &stackedWidget);
void createTopButtonsLayout(QHBoxLayout &topButtonsLayout, QStackedWidget &stackedWidget);
void createTextEditPage(QStackedWidget &stackedWidget, const QString &pageTitle, int buttonIndex, sqlite3* db);
void goToHomePage(QStackedWidget &stackedWidget);
void createRegisterPage(QStackedWidget &stackedWidget);
void createLoginPage(QStackedWidget &stackedWidget);


// Fonction pour créer la barre de menu
void createMenuBar(QMenuBar &menuBar, QStackedWidget &stackedWidget) {
    QMenu *fileMenu = menuBar.addMenu("File");

    QAction *openAction = fileMenu->addAction("Open");
    QObject::connect(openAction, &QAction::triggered, [&stackedWidget]() {
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Open File", QString(), "Text Files (*.txt);;All Files (*)");

        if (!filePath.isEmpty()) {
            QWidget *page = new QWidget;
            QVBoxLayout *layout = new QVBoxLayout(page);
            QTextEdit *textEdit = new QTextEdit(page);

            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                textEdit->setPlainText(in.readAll());
                file.close();

                layout->addWidget(textEdit);
                stackedWidget.addWidget(page);
                stackedWidget.setCurrentWidget(page);
            }
        }
    });

    QAction *saveAction = fileMenu->addAction("Save");
    // Connecter saveAction à une fonction pour sauvegarder le contenu, si nécessaire

    QAction *exitAction = fileMenu->addAction("Exit");
    // Connecter exitAction à une fonction pour quitter l'application, si nécessaire

    QMenu *optionsMenu = menuBar.addMenu("Options");

    // Add "Option 1" action
    QAction *option1Action = optionsMenu->addAction("Option 1");
    option1Action->setObjectName("option1Action");
    QObject::connect(option1Action, &QAction::triggered, [&stackedWidget]() {
        // Handling for "Option 1" action
        // ...
    });

    // Add "Option 2" action
    QAction *option2Action = optionsMenu->addAction("Option 2");
    option2Action->setObjectName("option2Action");
    QObject::connect(option2Action, &QAction::triggered, [&stackedWidget]() {
        // Handling for "Option 2" action
        // ...
    });

    // Create "Help" menu
    QMenu *helpMenu = menuBar.addMenu("Help");

    // Add "About" action
    QAction *aboutAction = helpMenu->addAction("About");
    aboutAction->setObjectName("aboutAction");
    QObject::connect(aboutAction, &QAction::triggered, [&stackedWidget]() {
        // Handling for "About" action
        // ...
    });
}



// Fonction pour créer le layout des boutons supérieurs
void createTopButtonsLayout(QHBoxLayout &topButtonsLayout, QStackedWidget &stackedWidget) {
    QPushButton *buttons[5];

    for (int i = 0; i < 5; ++i) {
        buttons[i] = new QPushButton("Button " + QString::number(i + 1));
        topButtonsLayout.addWidget(buttons[i]);

        QObject::connect(buttons[i], &QPushButton::clicked, [&stackedWidget, i]() {
            stackedWidget.setCurrentIndex(i);
        });
    }
}


// Fonction pour créer une page avec QTextEdit et un bouton "Send" uniquement pour le bouton 1
void createTextEditPage(QStackedWidget &stackedWidget, const QString &pageTitle, int buttonIndex, sqlite3* db) {
    QWidget *page = new QWidget;
    QVBoxLayout *pageLayout = new QVBoxLayout(page);

    if (buttonIndex == 0) {
        QLabel *labelSubject = new QLabel("Subject:", page);
        QLineEdit *lineEditSubject = new QLineEdit(page);
        lineEditSubject->setStyleSheet("margin-bottom: 5px;");

        QLabel *labelDescription = new QLabel("Description:", page);
        QLineEdit *lineEditDescription = new QLineEdit(page);
        lineEditDescription->setStyleSheet("margin-bottom: 5px;");

        QLabel *labelTag = new QLabel("Tag:", page);
        QLineEdit *lineEditTag = new QLineEdit(page);
        lineEditTag->setStyleSheet("margin-bottom: 5px;");

        QLabel *labelStatus = new QLabel("Status:", page);
        QLineEdit *lineEditStatus = new QLineEdit(page);
        lineEditStatus->setStyleSheet("margin-bottom: 5px;");

        QPushButton *sendButton = new QPushButton("Send", page);

        pageLayout->addWidget(labelSubject);
        pageLayout->addWidget(lineEditSubject);
        pageLayout->addWidget(labelDescription);
        pageLayout->addWidget(lineEditDescription);
        pageLayout->addWidget(labelTag);
        pageLayout->addWidget(lineEditTag);
        pageLayout->addWidget(labelStatus);
        pageLayout->addWidget(lineEditStatus);
        pageLayout->addWidget(sendButton);
        pageLayout->setSpacing(5);

        // Connect the button click signal to a slot for handling
        QObject::connect(sendButton, &QPushButton::clicked, [lineEditSubject, lineEditDescription, lineEditTag, lineEditStatus,db]() {
            // Handle the "Send" button click event
            qDebug() << "Send button clicked!";

            // Create a card with the entered values
            struct deck newdeck {
                    lineEditSubject->text().toUtf8().constData(),
                    lineEditDescription->text().toUtf8().constData(),
                    lineEditTag->text().toUtf8().constData(),
                    lineEditStatus->text().toUtf8().constData(),
                    1
            };
            int result = addDeck(db, &newdeck);
            if (result != 0){
                qDebug() << "bug";
            }
            else{
                qDebug() << "nobug";
            }


        });
    }else if (buttonIndex == 1) {
        // Page pour le bouton 2 (Mes Decks)
        QLabel *labelMesDecks = new QLabel("Mes Decks:", page);
        QTextEdit *textEditMesDecks = new QTextEdit(page);
        textEditMesDecks->setStyleSheet("margin-bottom: 5px;");
        textEditMesDecks->setPlainText("Liste de vos decks ici...");

        pageLayout->addWidget(labelMesDecks);
        pageLayout->addWidget(textEditMesDecks);
    } else if (buttonIndex == 2) {
        // Page pour le bouton 3 (Parcourir)
        QLabel *labelParcourir = new QLabel("Parcourir:", page);
        QTextEdit *textEditParcourir = new QTextEdit(page);
        textEditParcourir->setStyleSheet("margin-bottom: 5px;");
        textEditParcourir->setPlainText("Contenu du parcours ici...");

        pageLayout->addWidget(labelParcourir);
        pageLayout->addWidget(textEditParcourir);
    } else if (buttonIndex == 3) {
        // Page pour le bouton 4 (Statistiques)
        QLabel *labelStatistiques = new QLabel("Statistiques:", page);
        QTextEdit *textEditStatistiques = new QTextEdit(page);
        textEditStatistiques->setStyleSheet("margin-bottom: 5px;");
        textEditStatistiques->setPlainText("Statistiques ici...");

        pageLayout->addWidget(labelStatistiques);
        pageLayout->addWidget(textEditStatistiques);
    } else if (buttonIndex == 4) {
        // Page pour le bouton 5 (Page sans contenu défini)
        QLabel *labelPlaceholder = new QLabel("Contenu à définir pour le bouton 5.", page);

        pageLayout->addWidget(labelPlaceholder);
    }

    stackedWidget.addWidget(page);
}




void goToHomePage(QStackedWidget &stackedWidget) {
    stackedWidget.setCurrentIndex(0); // Index 0 correspond à la page d'accueil
}


// Fonction pour créer une page de connexion
void createLoginPage(QStackedWidget &stackedWidget) {
    QWidget *loginPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(loginPage);

    QLabel *labelUsername = new QLabel("Username:", loginPage);
    QLineEdit *usernameLineEdit = new QLineEdit(loginPage);

    QLabel *labelPassword = new QLabel("Password:", loginPage);
    QLineEdit *passwordLineEdit = new QLineEdit(loginPage);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton *loginButton = new QPushButton("Login", loginPage);

    // Créez un lien "Register"
    QLabel *registerLink = new QLabel("<a href=\"#\">Register</a>", loginPage);
    registerLink->setTextFormat(Qt::RichText);
    registerLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    registerLink->setOpenExternalLinks(false);

    // Connectez le signal du clic sur le lien "Register" à la fonction pour afficher la page d'enregistrement
    QObject::connect(registerLink, &QLabel::linkActivated, [&stackedWidget]() {
        // Changez la page d'enregistrement (index 2 dans cet exemple)
        stackedWidget.setCurrentIndex(6);
    });

    layout->addWidget(labelUsername);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(labelPassword);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
    layout->addWidget(registerLink);  // Ajoutez le lien "Register" au layout

    // Capture explicite des variables utilisées dans la lambda
    QObject::connect(loginButton, &QPushButton::clicked, [&stackedWidget, usernameLineEdit, passwordLineEdit, loginPage]() {
        // Handle the "Login" button click event
        qDebug() << "Login button clicked!";
        qDebug() << "Username: " << usernameLineEdit->text();
        qDebug() << "Password: " << passwordLineEdit->text();

        // Add your login logic here, e.g., check credentials
        // For now, let's just show a message box indicating success
        QMessageBox::information(loginPage, "Login Successful", "Welcome!");

        // You can add logic to switch to another page after successful login
        stackedWidget.setCurrentIndex(7);
    });

    stackedWidget.addWidget(loginPage);
}



void createRegisterPage(QStackedWidget &stackedWidget) {
    QWidget *registerPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(registerPage);

    QLabel *labelUsername = new QLabel("Username:", registerPage);
    QLineEdit *usernameLineEdit = new QLineEdit(registerPage);

    QLabel *labelPassword = new QLabel("Password:", registerPage);
    QLineEdit *passwordLineEdit = new QLineEdit(registerPage);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QLabel *labelConfirmPassword = new QLabel("Confirm Password:", registerPage);
    QLineEdit *confirmPasswordLineEdit = new QLineEdit(registerPage);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton *registerButton = new QPushButton("Register", registerPage);

    QLabel *LoginLink = new QLabel("<a href=\"#\">Back to Login</a>", registerPage);
    LoginLink->setTextFormat(Qt::RichText);
    LoginLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    LoginLink->setOpenExternalLinks(false);

    // Connectez le signal du clic sur le lien "Register" à la fonction pour afficher la page d'enregistrement
    QObject::connect(LoginLink, &QLabel::linkActivated, [&stackedWidget]() {
        // Changez la page d'enregistrement (index 2 dans cet exemple)
        stackedWidget.setCurrentIndex(5);
    });

    layout->addWidget(labelUsername);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(labelPassword);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(labelConfirmPassword);
    layout->addWidget(confirmPasswordLineEdit);
    layout->addWidget(registerButton);
    layout->addWidget(LoginLink);

    // Capture explicite des variables utilisées dans la lambda
    QObject::connect(registerButton, &QPushButton::clicked, [&stackedWidget, usernameLineEdit, passwordLineEdit, confirmPasswordLineEdit, registerPage]() {
        // Handle the "Register" button click event
        qDebug() << "Register button clicked!";
        qDebug() << "Username: " << usernameLineEdit->text();
        qDebug() << "Password: " << passwordLineEdit->text();
        qDebug() << "Confirm Password: " << confirmPasswordLineEdit->text();

        // Add your registration logic here, e.g., validate input
        // For now, let's just show a message box indicating success
        QMessageBox::information(registerPage, "Registration Successful", "You are now registered!");

        // You can add logic to switch to another page after successful registration
        stackedWidget.setCurrentIndex(5); // Changez à la page de connexion (index 5 dans cet exemple)
    });

    stackedWidget.addWidget(registerPage);
}





