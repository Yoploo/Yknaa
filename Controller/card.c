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

struct Cardlist* addCardToList(struct Cardlist* head, int card_id, const char* recto, const char* verso, int rank, int points) {
    struct Cardlist* newCard = (struct Cardlist*)malloc(sizeof(struct Cardlist));
    if(!newCard) {
        exit(EXIT_FAILURE);
    }
    newCard->card_id = card_id;
    newCard->recto = recto;
    newCard->verso = verso;
    newCard->rank = rank;
    newCard->points = points;
    newCard->next = head;
    return newCard;
}


struct Cardlist* cardsByDeckId(sqlite3* db, int deck_id) {
    const char* cards = "SELECT card_id, recto, verso, rank, points FROM cards WHERE deck_id = ?";
    sqlite3_stmt* stmt;

    int returnCards = sqlite3_prepare_v2(db, cards, -1, &stmt, 0);
    if (returnCards != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la préparation de la requête pour récupérer les cartes : %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_int(stmt, 1, deck_id);

    struct Cardlist* cardList = NULL;

    int req;
    while ((req = sqlite3_step(stmt)) == SQLITE_ROW) {
        int card_id = sqlite3_column_int(stmt, 0);
        const char* recto = (const char*)sqlite3_column_text(stmt, 1);
        const char* verso = (const char*)sqlite3_column_text(stmt, 2);
        int rank = sqlite3_column_int(stmt, 3);
        int points = sqlite3_column_int(stmt, 4);

        cardList = addCardToList(cardList, card_id, recto, verso, rank, points);
    }

    if (req != SQLITE_DONE) {
        fprintf(stderr, "Erreur lors de l'exécution de la requête pour récupérer les cartes : %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return NULL;
    }

    sqlite3_finalize(stmt);

    return cardList;
}

void freeCardList(struct Cardlist* head) {
    while (head != NULL) {
        struct Cardlist* temp = head;
        head = head->next;
        free((char*)temp->recto);
        free((char*)temp->verso);
        free(temp);
    }
}