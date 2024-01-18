//
// Created by loloy on 18/01/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "deck.h"

int addDeck(sqlite3* db, const struct deck* Deck);
int deleteDeck(sqlite3* db, const struct deck* Deck, int deck_id);

int addDeck(sqlite3* db, const struct deck* Deck){
    // Préparez la requête SQL avec des paramètres préparés
    const char* sql = "INSERT INTO decks (subject, description, tag, status, user_id) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;

    // Préparez la requête SQL
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Liez les valeurs aux paramètres préparés
    sqlite3_bind_text(stmt, 1, Deck->subject, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, Deck->description, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, Deck->tag, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, Deck->status, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, Deck->user_id);

    // Exécutez la requête préparée
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Finalisez la requête préparée
    sqlite3_finalize(stmt);

    printf("Deck ajouté avec succès.\n");

    return 0;
}

int deleteDeck(sqlite3* db, const struct deck* Deck, int deck_id) {
    const char* sql = "DELETE FROM decks WHERE deck_id = ? AND user_id = ?";

    sqlite3_stmt* stmt;

    int req = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    //gestion erreur
    if(req != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_bind_int(stmt, 1, deck_id);
    sqlite3_bind_int(stmt, 2, Deck->user_id);

    req = sqlite3_step(stmt);
    if(req != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'execution de la requete : %s\n", sqlite3_errmsg(db));
        return req;
    }

    sqlite3_finalize(stmt);

    if (sqlite3_changes(db) == 0) {
        printf("Le deck n'appartient pas à l'utilisateur connecté.\n");
        return SQLITE_ERROR;
    }


    printf("Deck supprimé avec succès.\n");

    return 0;

}

int editDeck(sqlite3* db, int deck_id, int user_id) {
}