//
// Created by loloy on 18/01/2024.
//
#include <stdio.h>
#include <sqlite3.h>

// Fonction pour créer la base de données et exécuter les requêtes SQL
int createDatabase() {
    sqlite3 *db;
    int rc;

    // Ouvrir la base de données (elle sera créée si elle n'existe pas)
    rc = sqlite3_open("Ykna.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Impossible d'ouvrir la base de données : %s\n", sqlite3_errmsg(db));
        return rc;
    }

    // Exécuter les requêtes SQL
    const char *sql1 = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, nickname TEXT NOT NULL, password TEXT NOT NULL, rank INTEGER);";
    const char *sql2 = "CREATE TABLE IF NOT EXISTS decks (deck_id INTEGER PRIMARY KEY, subject TEXT NOT NULL, description TEXT NOT NULL, tag TEXT, status TEXT, user_id INTEGER, FOREIGN KEY (user_id) REFERENCES users(id));";
    const char *sql3 = "CREATE TABLE IF NOT EXISTS cards (card_id INTEGER PRIMARY KEY, recto TEXT NOT NULL, verso TEXT NOT NULL, rank INTEGER, points INTEGER, deck_id INTEGER, user_id INTEGER, FOREIGN KEY (user_id) REFERENCES users(id), FOREIGN KEY (deck_id) REFERENCES decks(deck_id));";

    rc = sqlite3_exec(db, sql1, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table users : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    rc = sqlite3_exec(db, sql2, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table decks : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    rc = sqlite3_exec(db, sql3, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de la création de la table cards : %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // Fermer la base de données
    sqlite3_close(db);

    printf("Base de données créée avec succès.\n");

    return 0;
}
