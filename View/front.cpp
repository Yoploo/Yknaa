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
#include <QTextBrowser>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QLineEdit>
#include <sqlite3.h>
#include "../Controller/deck.h"
#include "../Controller/deck.c"
#include "../Controller/card.h"
#include "../Controller/card.c"
#include "../Controller/user.h"
#include "../Controller/user.c"

void createMenuBar(QMenuBar &menuBar, QStackedWidget &stackedWidget);
void createTopButtonsLayout(QHBoxLayout &topButtonsLayout, QStackedWidget &stackedWidget);
void createTextEditPage(QStackedWidget &stackedWidget, const QString &pageTitle, int buttonIndex, sqlite3* db,int user_id);
void goToHomePage(QStackedWidget &stackedWidget);
void createRegisterPage(QStackedWidget &stackedWidget,sqlite3 *db);
int createLoginPage(QStackedWidget &stackedWidget, sqlite3 *db, struct user *myUser, QWidget &loginPage);
void freeCardList(struct Cardlist* head);
struct Cardlist* cardsByDeckId(sqlite3* db, int deck_id);
void createDeckPage(QStackedWidget &stackedWidget, int deck_id,int user_id, sqlite3* db);
int addCard(sqlite3* db, const struct card *Card);
void updateCardList(QWidget *deckPage, sqlite3 *db, int deck_id, int user_id);
void clearCardListWidget(QWidget *cardListWidget);


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

    QAction *exitAction = fileMenu->addAction("Exit");


    QMenu *optionsMenu = menuBar.addMenu("Options");


    QAction *option1Action = optionsMenu->addAction("Option 1");
    option1Action->setObjectName("option1Action");
    QObject::connect(option1Action, &QAction::triggered, [&stackedWidget]() {

    });


    QAction *option2Action = optionsMenu->addAction("Option 2");
    option2Action->setObjectName("option2Action");
    QObject::connect(option2Action, &QAction::triggered, [&stackedWidget]() {

    });


    QMenu *helpMenu = menuBar.addMenu("Help");


    QAction *aboutAction = helpMenu->addAction("About");
    aboutAction->setObjectName("aboutAction");
    QObject::connect(aboutAction, &QAction::triggered, [&stackedWidget]() {

    });
}



// Fonction pour créer le layout des boutons supérieurs
void createTopButtonsLayout(QHBoxLayout &topButtonsLayout, QStackedWidget &stackedWidget) {
    QPushButton *buttons[5];

    for (int i = 0; i < 5; ++i) {
        buttons[i] = new QPushButton("Button " + QString::number(i + 1));
        topButtonsLayout.addWidget(buttons[i]);

        QObject::connect(buttons[i], &QPushButton::clicked, [&stackedWidget, i]() {
            stackedWidget.setCurrentIndex(i+3);
        });
    }
}


// Fonction pour créer une page avec QTextEdit et un bouton "Send" uniquement pour le bouton 1
void createTextEditPage(QStackedWidget &stackedWidget, const QString &pageTitle, int buttonIndex, sqlite3* db,int user_id) {
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


        QObject::connect(sendButton, &QPushButton::clicked, [lineEditSubject, lineEditDescription, lineEditTag, lineEditStatus,db,user_id]() {

            qDebug() << "Send button clicked!";

            struct deck newdeck {
                    lineEditSubject->text().toUtf8().constData(),
                    lineEditDescription->text().toUtf8().constData(),
                    lineEditTag->text().toUtf8().constData(),
                    lineEditStatus->text().toUtf8().constData(),
                    user_id

            };
            int result = addDeck(db, &newdeck);
            if (result != 0){
                qDebug() << "bug";
            }
            else{
                qDebug() << "nobug";
            }


        });
    }else
        QString savedText;
        if (buttonIndex == 1) {
        // Page pour le bouton 2 (Mes Decks)
        QLabel *labelMesDecks = new QLabel("Mes Decks:", page);
        QTextBrowser *textEditMesDecks = new QTextBrowser(page);
        textEditMesDecks->setStyleSheet("margin-bottom: 5px;");



        struct decklist* deckList = decksByUserId(db, user_id);


        if (deckList != NULL) {

            while (deckList != NULL) {

                QString deckInfo = QString("Subject: %1\nDescription: %2\nTag: %3\nStatus: %4\n\n")
                        .arg(deckList->subject)
                        .arg(deckList->description)
                        .arg(deckList->tag)
                        .arg(deckList->status);

                textEditMesDecks->append(deckInfo);

                // Utilisez un lien pour afficher les cartes du deck
                QString deckLink = QString("<a href=\"deck://%1\">Voir les cartes du deck</a>").arg(deckList->deck_id);

                QObject::connect(textEditMesDecks, &QTextBrowser::anchorClicked, [deckList,user_id, db, &stackedWidget](const QUrl& link) {
                    if (link.scheme() == "deck") {
                        int deck_id = deckList->deck_id;
                        createDeckPage(stackedWidget, deck_id,user_id, db);
                    }
                });
                textEditMesDecks->append(deckLink);


                textEditMesDecks->append("------------------------------");

                deckList = deckList->next;
            }


            freeDeckList(deckList);
        } else {
            textEditMesDecks->setPlainText("Aucun deck trouvé.");
        }

        pageLayout->addWidget(labelMesDecks);
        pageLayout->addWidget(textEditMesDecks);
    } else if (buttonIndex == 2) {

        QLabel *labelParcourir = new QLabel("Parcourir:", page);
        QTextEdit *textEditParcourir = new QTextEdit(page);
        textEditParcourir->setStyleSheet("margin-bottom: 5px;");
        textEditParcourir->setPlainText("Contenu du parcours ici...");

        pageLayout->addWidget(labelParcourir);
        pageLayout->addWidget(textEditParcourir);
    } else if (buttonIndex == 3) {

        QLabel *labelStatistiques = new QLabel("Statistiques:", page);
        QTextEdit *textEditStatistiques = new QTextEdit(page);
        textEditStatistiques->setStyleSheet("margin-bottom: 5px;");
        textEditStatistiques->setPlainText("Statistiques ici...");

        pageLayout->addWidget(labelStatistiques);
        pageLayout->addWidget(textEditStatistiques);
    } else if (buttonIndex == 4) {

        QLabel *labelPlaceholder = new QLabel("Contenu à définir pour le bouton 5.", page);

        pageLayout->addWidget(labelPlaceholder);
    }

    stackedWidget.addWidget(page);
}






int createLoginPage(QStackedWidget &stackedWidget, sqlite3 *db, struct user *myUser, QWidget &loginPage) {
    QVBoxLayout *layout = new QVBoxLayout(&loginPage);

    QLabel *labelUsername = new QLabel("Username:", &loginPage);
    QLineEdit *usernameLineEdit = new QLineEdit(&loginPage);

    QLabel *labelPassword = new QLabel("Password:", &loginPage);
    QLineEdit *passwordLineEdit = new QLineEdit(&loginPage);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton *loginButton = new QPushButton("Login", &loginPage);


    QLabel *registerLink = new QLabel("<a href=\"#\">Register</a>", &loginPage);
    registerLink->setTextFormat(Qt::RichText);
    registerLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    registerLink->setOpenExternalLinks(false);


    QObject::connect(registerLink, &QLabel::linkActivated, [&stackedWidget]() {

        stackedWidget.setCurrentIndex(1);
    });

    layout->addWidget(labelUsername);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(labelPassword);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
    layout->addWidget(registerLink);


    QObject::connect(loginButton, &QPushButton::clicked, [&stackedWidget, usernameLineEdit, passwordLineEdit, &loginPage, db, myUser]() {

        qDebug() << "Login button clicked!";
        qDebug() << "Username: " << usernameLineEdit->text();
        qDebug() << "Password: " << passwordLineEdit->text();

        myUser->nickname = usernameLineEdit->text().toUtf8().constData();
        myUser->password = passwordLineEdit->text().toUtf8().constData();

        int res = loginUser(db, myUser);
        if (res == 0) {
            QMessageBox::information(&loginPage, "Login Failed", "Wrong nickname or password");
            return 0;
        }else {
            QMessageBox::information(&loginPage, "Login Successful", "Welcome!");
            stackedWidget.setCurrentIndex(2);
            // Create pages for each button
            for (int i = 1; i <= 5; ++i) {
                createTextEditPage(stackedWidget, "Page for Button " + QString::number(i), i - 1, db, res);
            }

            return res;
        }
    });

    stackedWidget.addWidget(&loginPage);
    return 0;
}



void createRegisterPage(QStackedWidget &stackedWidget,sqlite3 *db) {
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


    QObject::connect(LoginLink, &QLabel::linkActivated, [&stackedWidget]() {

        stackedWidget.setCurrentIndex(0);
    });

    layout->addWidget(labelUsername);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(labelPassword);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(labelConfirmPassword);
    layout->addWidget(confirmPasswordLineEdit);
    layout->addWidget(registerButton);
    layout->addWidget(LoginLink);


    QObject::connect(registerButton, &QPushButton::clicked, [&stackedWidget, usernameLineEdit, passwordLineEdit, confirmPasswordLineEdit, registerPage,db]() {

        qDebug() << "Register button clicked!";
        qDebug() << "Username: " << usernameLineEdit->text();
        qDebug() << "Password: " << passwordLineEdit->text();
        qDebug() << "Confirm Password: " << confirmPasswordLineEdit->text();




        struct user newuser{
                -1,
                usernameLineEdit->text().toUtf8().constData(),
                passwordLineEdit->text().toUtf8().constData(),
                0

        };
        if(verifpassword(passwordLineEdit->text().toUtf8().constData(),confirmPasswordLineEdit->text().toUtf8().constData()) == 1){
            int result = registerUser(db,&newuser);
            if(result == 0){
                QMessageBox::information(registerPage, "Registration Failed", "The nickname already exist");
            }
            else{
                QMessageBox::information(registerPage, "Registration Successful", "You are now registered!");
                stackedWidget.setCurrentIndex(0); // Changez à la page de connexion (index 5 dans cet exemple)
            }
        }
        else{
            QMessageBox::information(registerPage, "Registration Failed", "Confirmation password is not the same as the password");
        }
    });

    stackedWidget.addWidget(registerPage);
}

void createDeckPage(QStackedWidget &stackedWidget, int deck_id, int user_id, sqlite3 *db) {


    QWidget *deckPage = new QWidget;
    QVBoxLayout *deckPageLayout = new QVBoxLayout(deckPage);


    struct Cardlist *cardList = cardsByDeckId(db, deck_id);




    if (cardList != NULL) {
        printf("%s",cardList->recto);
        while (cardList != NULL) {

            QString cardInfo = QString("Recto: %1\nVerso: %2\nRank: %3\nPoints: %4\n\n")
                    .arg(cardList->recto)
                    .arg(cardList->verso)
                    .arg(cardList->rank)
                    .arg(cardList->points);
            QLabel *cardLabel = new QLabel(cardInfo, deckPage);
            deckPageLayout->addWidget(cardLabel);

            cardList = cardList->next;
        }


        freeCardList(cardList);
    } else {
        QLabel *noCardsLabel = new QLabel("Aucune carte trouvée pour ce deck.", deckPage);
        deckPageLayout->addWidget(noCardsLabel);
    }


    QLabel *labelAddCard = new QLabel("Ajouter une carte au deck:", deckPage);


    QLabel *labelRecto = new QLabel("Recto:", deckPage);
    QLineEdit *lineEditRecto = new QLineEdit(deckPage);

    QLabel *labelVerso = new QLabel("Verso:", deckPage);
    QLineEdit *lineEditVerso = new QLineEdit(deckPage);

    QLabel *labelRank = new QLabel("Rank:", deckPage);
    QLineEdit *lineEditRank = new QLineEdit(deckPage);

    QLabel *labelPoints = new QLabel("Points:", deckPage);
    QLineEdit *lineEditPoints = new QLineEdit(deckPage);

    QPushButton *addCardButton = new QPushButton("Ajouter la carte", deckPage);


    QObject::connect(addCardButton, &QPushButton::clicked, [db, deck_id, user_id, lineEditRecto, lineEditVerso, lineEditRank, lineEditPoints, deckPage]() {

        const char *recto = lineEditRecto->text().toUtf8().constData();
        const char *verso = lineEditVerso->text().toUtf8().constData();
        int rank = lineEditRank->text().toInt();
        int points = lineEditPoints->text().toInt();

        struct card newCard{
                recto,
                verso,
                rank,
                points,
                deck_id,
                user_id
        };


        int result = addCard(db, &newCard);

        if (result != 0) {
            qDebug() << "Erreur lors de l'ajout de la carte";
        } else {
            qDebug() << "Carte ajoutée avec succès";

            updateCardList(deckPage, db, deck_id,user_id);
        }
    });

    deckPageLayout->addWidget(labelAddCard);


    deckPageLayout->addWidget(labelRecto);
    deckPageLayout->addWidget(lineEditRecto);

    deckPageLayout->addWidget(labelVerso);
    deckPageLayout->addWidget(lineEditVerso);

    deckPageLayout->addWidget(labelRank);
    deckPageLayout->addWidget(lineEditRank);

    deckPageLayout->addWidget(labelPoints);
    deckPageLayout->addWidget(lineEditPoints);

    deckPageLayout->addWidget(addCardButton);


    stackedWidget.addWidget(deckPage);


    stackedWidget.setCurrentWidget(deckPage);
}

void updateCardList(QWidget *deckPage, sqlite3 *db, int deck_id, int user_id) {

    QLayoutItem *child;
    while ((child = deckPage->layout()->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }


    struct Cardlist *cardList = cardsByDeckId(db, deck_id);


    if (cardList != NULL) {

        while (cardList != NULL) {

            QString cardInfo = QString("Recto: %1\nVerso: %2\nRank: %3\nPoints: %4\n\n")
                    .arg(cardList->recto)
                    .arg(cardList->verso)
                    .arg(cardList->rank)
                    .arg(cardList->points);

            QLabel *cardLabel = new QLabel(cardInfo, deckPage);
            deckPage->layout()->addWidget(cardLabel);

            cardList = cardList->next;
        }


        freeCardList(cardList);
    } else {
        QLabel *noCardsLabel = new QLabel("Aucune carte trouvée pour ce deck.", deckPage);
        deckPage->layout()->addWidget(noCardsLabel);
    }


    QLabel *labelAddCard = new QLabel("Ajouter une carte au deck:", deckPage);


    QLabel *labelRecto = new QLabel("Recto:", deckPage);
    QLineEdit *lineEditRecto = new QLineEdit(deckPage);

    QLabel *labelVerso = new QLabel("Verso:", deckPage);
    QLineEdit *lineEditVerso = new QLineEdit(deckPage);

    QLabel *labelRank = new QLabel("Rank:", deckPage);
    QLineEdit *lineEditRank = new QLineEdit(deckPage);

    QLabel *labelPoints = new QLabel("Points:", deckPage);
    QLineEdit *lineEditPoints = new QLineEdit(deckPage);

    QPushButton *addCardButton = new QPushButton("Ajouter la carte", deckPage);


    QObject::connect(addCardButton, &QPushButton::clicked, [db, deck_id, user_id, lineEditRecto, lineEditVerso, lineEditRank, lineEditPoints, deckPage]() {

        const char *recto = lineEditRecto->text().toUtf8().constData();
        const char *verso = lineEditVerso->text().toUtf8().constData();
        int rank = lineEditRank->text().toInt();
        int points = lineEditPoints->text().toInt();

        struct card newCard{
                recto,
                verso,
                rank,
                points,
                deck_id,
                user_id
        };


        int result = addCard(db, &newCard);

        if (result != 0) {
            qDebug() << "Erreur lors de l'ajout de la carte";
        } else {
            qDebug() << "Carte ajoutée avec succès";

            updateCardList(deckPage, db, deck_id, user_id);
        }
    });

    deckPage->layout()->addWidget(labelAddCard);


    deckPage->layout()->addWidget(labelRecto);
    deckPage->layout()->addWidget(lineEditRecto);

    deckPage->layout()->addWidget(labelVerso);
    deckPage->layout()->addWidget(lineEditVerso);

    deckPage->layout()->addWidget(labelRank);
    deckPage->layout()->addWidget(lineEditRank);

    deckPage->layout()->addWidget(labelPoints);
    deckPage->layout()->addWidget(lineEditPoints);

    deckPage->layout()->addWidget(addCardButton);


    deckPage->layout()->update();
}




void clearCardListWidget(QWidget *cardListWidget) {
    QLayout *layout = cardListWidget->layout();
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}







