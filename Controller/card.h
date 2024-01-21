//
// Created by loloy on 18/01/2024.
//

#ifndef YKNAA_CARD_H
#define YKNAA_CARD_H

struct card {
    const char* recto;
    const char* verso;
    int rank;
    int points;
    int deck_id;
    int user_id;
};

struct Cardlist {
    int card_id;
    const char* recto;
    const char* verso;
    int rank;
    int points;

    struct Cardlist* next;
};


void freeCardList(struct Cardlist* head);
struct Cardlist* cardsByDeckId(sqlite3* db, int deck_id);
int addCard(sqlite3* db, const struct card *Card);

#endif //YKNAA_CARD_H
