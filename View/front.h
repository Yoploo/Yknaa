//
// Created by loloy on 18/01/2024.
//

#ifndef YKNAA_FRONT_H
#define YKNAA_FRONT_H


#include <QMenuBar>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QString>


void createMenuBar(QMenuBar &menuBar, QStackedWidget &stackedWidget);
void createTopButtonsLayout(QHBoxLayout &topButtonsLayout, QStackedWidget &stackedWidget);
void createTextEditPage(QStackedWidget &stackedWidget, const QString &pageTitle, int buttonIndex, sqlite3* db);
void goToHomePage(QStackedWidget &stackedWidget);
void createLoginPage(QStackedWidget &stackedWidget,sqlite3 *db,struct user* myUser);
void createRegisterPage(QStackedWidget &stackedWidget,sqlite3 *db);

#endif //YKNAA_FRONT_H
