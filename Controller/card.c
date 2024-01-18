//
// Created by loloy on 18/01/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "card.h"


int addCard(sqlite3* db, const struct card *Card);
int deleteCard(sqlite3* db, const struct card *Card, int card_id);

int addCard(sqlite3* db, const struct card *Card) {
    //partie verif de deck (à refaire)
    const char* checkDeck = "SELECT COUNT(*) FROM decks WHERE deck_id = ? AND user_id = ?";
    sqlite3_stmt* ownerdeckStmt;

    int ownershipReq = sqlite3_prepare_v2(db, checkDeck, -1, &ownerdeckStmt, 0);
    if (ownershipReq != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête de vérification de l'appartenance au deck : %s\n", sqlite3_errmsg(db));
        return ownershipReq; // ou toute autre valeur pour indiquer une erreur
    }

    sqlite3_bind_int(ownerdeckStmt, 1, Card->deck_id);
    sqlite3_bind_int(ownerdeckStmt, 2, Card->user_id);

    ownershipReq = sqlite3_step(ownerdeckStmt);
    if (ownershipReq != SQLITE_ROW) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête de vérification de l'appartenance au deck : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(ownerdeckStmt);
        return ownershipReq; // ou toute autre valeur pour indiquer une erreur
    }

    int count = sqlite3_column_int(ownerdeckStmt, 0);
    sqlite3_finalize(ownerdeckStmt);
    if (count == 0) {
        printf("Erreur : Vous ne pouvez pas ajouter une carte à un deck qui n'appartient pas à l'utilisateur.\n");
        return -1; // ou toute autre valeur pour indiquer une erreur
    }



    //partie insertion de donnée
    const char* sql = "INSERT INTO cards (recto, verso, rank, points, deck_id, user_id) VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;

    int req = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (req != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_bind_text(stmt, 1, Card->recto, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, Card->verso, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, Card->rank);
    sqlite3_bind_int(stmt, 4, Card->points);
    sqlite3_bind_int(stmt, 5, Card->deck_id);
    sqlite3_bind_int(stmt, 6, Card->user_id);

    req = sqlite3_step(stmt);
    if (req != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_finalize(stmt);
    printf("carte ajoutée avec succès.\n");

    return 0;

}

int deleteCard(sqlite3* db, const struct card *Card,int card_id) {
    const char* sql = "DELETE FROM cards WHERE card_id = ? AND user_id = ?";

    sqlite3_stmt* stmt;

    int req = sqlite3_prepare_v2(db, sql, -1, &stmt, 0); //important
    if(req != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_bind_int(stmt, 1, card_id);
    sqlite3_bind_int(stmt, 2, Card->user_id);

    req = sqlite3_step(stmt);
    if(req != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_finalize(stmt);
    if (sqlite3_changes(db) == 0) {
        printf("La carte n'appartient pas à l'utilisateur connecté.\n");
        return SQLITE_ERROR;
    }

    printf("Carte supprimé avec succès.\n");

    return 0;


}