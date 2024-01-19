//
// Created by Mathéo on 19/01/2024.
//
#include <stdio.h>
#include <sqlite3.h>
#include "user.h"
int registerUser(sqlite3* db, const struct user *User);
int UserExists(sqlite3* db, const char* nickname);
int loginUser(sqlite3* db, const struct user *User);
int deckByUser(sqlite3* db, const struct user *User);

int userExists(sqlite3* db, const char* nickname) {
    // Requête pour vérifier si l'utilisateur existe déjà
    const char* checkUserSQL = "SELECT COUNT(*) FROM users WHERE nickname = ?";
    sqlite3_stmt* userStmt;

    int userReq = sqlite3_prepare_v2(db, checkUserSQL, -1, &userStmt, 0);
    if (userReq != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête de vérification de l'existence de l'utilisateur : %s\n", sqlite3_errmsg(db));
        return -1; // ou toute autre valeur pour indiquer une erreur
    }

    sqlite3_bind_text(userStmt, 1, nickname, -1, SQLITE_STATIC);

    userReq = sqlite3_step(userStmt);
    if (userReq != SQLITE_ROW) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête de vérification de l'existence de l'utilisateur : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(userStmt);
        return -1; // ou toute autre valeur pour indiquer une erreur
    }

    int count = sqlite3_column_int(userStmt, 0);
    sqlite3_finalize(userStmt);

    return count; // Si count est 0, l'utilisateur n'existe pas, sinon, il existe
}

int registerUser(sqlite3* db, const struct user *User) {

    // Vérifier si l'utilisateur existe déjà
    int userCount = userExists(db, User->nickname);

    if (userCount < 0) {
        fprintf(stderr, "Erreur lors de la vérification de l'existence de l'utilisateur.\n");
        return userCount; // ou toute autre valeur pour indiquer une erreur
    } else if (userCount > 0) {
        fprintf(stderr, "L'utilisateur avec le nom '%s' existe déjà.\n", User->nickname);
        return -1; // ou toute autre valeur pour indiquer que l'utilisateur existe déjà
    }

    if (db == NULL || User == NULL || User->nickname == NULL) {
        fprintf(stderr, "Erreur : Arguments non valides.\n");
        return -1; // Code d'erreur à votre choix
    }

    //partie insertion de donnée
    const char* sql = "INSERT INTO users (nickname, password, rank) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;

    int req = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (req != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_bind_text(stmt, 1, User->nickname, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, User->password);
    sqlite3_bind_int(stmt, 3, User->rank);

    req = sqlite3_step(stmt);
    if (req != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_finalize(stmt);
    printf("Utilisateur ajoutée avec succès.\n");

    return 0;

}

int loginUser(sqlite3* db, const struct user *User) {

    // Vérifier si l'utilisateur existe déjà
    int userCount = userExists(db, User->nickname);

    if (userCount < 0) {
        fprintf(stderr, "Erreur lors de la vérification de l'existence de l'utilisateur.\n");
        return userCount; // ou toute autre valeur pour indiquer une erreur
    } else if (userCount > 0) {
        fprintf(stderr, "L'utilisateur avec le nom '%s' existe déjà.\n", User->nickname);
        return -1; // ou toute autre valeur pour indiquer que l'utilisateur existe déjà
    }

    const char* checkPassword = "SELECT password FROM users WHERE user_id = ?";
    sqlite3_stmt* stmt;

    int req = sqlite3_prepare_v2(db, checkPassword, -1, &stmt, 0);
    if (req != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_bind_int(stmt, 1, User->user_id);
    sqlite3_bind_int(stmt, 2, User->password);

    req = sqlite3_step(stmt);
    if (req != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_finalize(stmt);

    return 0;
}

int deckByUser(sqlite3* db, const struct user *User) {
    const char* Decks = "SELECT deck_id FROM decks WHERE user_id = ?";
    sqlite3_stmt* stmt;

    int req = sqlite3_prepare_v2(db, Decks, -1, &stmt, 0);
    if (req != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_bind_int(stmt, 1, User->user_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int deck_id = sqlite3_column_int(stmt, 0);
        return deck_id;
    }

    sqlite3_finalize(stmt);

    return 0;
}