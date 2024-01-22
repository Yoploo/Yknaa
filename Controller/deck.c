//
// Created by loloy on 18/01/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "deck.h"
#include <string.h>

int addDeck(sqlite3* db, const struct deck* Deck);
int deleteDeck(sqlite3* db, const struct deck* Deck, int deck_id);
struct decklist* addDeckToList(struct decklist* head, int deck_id, const char* subject, const char* description, const char* tag, const char* status);
struct decklist* decksByUserId(sqlite3* db, int user_id);
void freeDeckList(struct decklist* deckList);
int editDeck(sqlite3* db, int deck_id, int user_id);

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

struct decklist* addDeckToList(struct decklist* head, int deck_id, const char* subject, const char* description, const char* tag, const char* status) {
    struct decklist* newDeck = (struct decklist*)malloc(sizeof(struct decklist));
    if (!newDeck) {
        exit(EXIT_FAILURE);
    }
    newDeck->deck_id = deck_id;
    newDeck->subject = strdup(subject);
    newDeck->description = strdup(description);
    newDeck->tag = strdup(tag);
    newDeck->status = strdup(status);
    newDeck->next = head;
    return newDeck;
}

struct decklist* decksByUserId(sqlite3* db, int user_id) {
    const char* decks = "SELECT deck_id, subject, description, tag, status FROM decks WHERE user_id = ?";
    sqlite3_stmt* stmt;

    int returnDecks = sqlite3_prepare_v2(db, decks, -1, &stmt, 0);

    if (returnDecks != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête : %s\n", sqlite3_errmsg(db));
        return NULL;
    }


    sqlite3_bind_int(stmt, 1, user_id);

    struct decklist* deckList = NULL;


    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int deck_id = sqlite3_column_int(stmt, 0);
        const char* subject = (const char*)sqlite3_column_text(stmt, 1);
        const char* description = (const char*)sqlite3_column_text(stmt, 2);
        const char* tag = (const char*)sqlite3_column_text(stmt, 3);
        const char* status = (const char*)sqlite3_column_text(stmt, 4);

        deckList = addDeckToList(deckList, deck_id, subject, description, tag, status);
    }


    if (sqlite3_errcode(db) != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête pour récupérer les deck_id : %s\n", sqlite3_errmsg(db));
        freeDeckList(deckList);
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);
    return deckList;
}



void freeDeckList(struct decklist* deckList) {
    while (deckList != NULL) {
        struct decklist* temp = deckList;
        deckList = deckList->next;


        free((char*)temp->subject);
        free((char*)temp->description);
        free((char*)temp->tag);
        free((char*)temp->status);


        free(temp);
    }
}


int editDeck(sqlite3* db, int deck_id, int user_id) {
    return 0;
}