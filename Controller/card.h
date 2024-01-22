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
    const char* recto;
    const char* verso;
    int rank;
    int points;

    struct Cardlist* next;
};


int addCard(sqlite3* db, const struct card *Card);
int deleteCard(sqlite3* db, const struct card *Card, int card_id);
struct Cardlist* addCardToList(struct Cardlist* head,const char* recto, const char* verso, int rank, int points);
struct Cardlist* addCardListToCardListAtIndex(struct Cardlist* head, const struct Cardlist* listToAdd, int index);
struct Cardlist* readCardListFromFile(const char* fileName);
struct Cardlist* createOrReadDeckList(int deck_id, sqlite3* db);
struct Cardlist* cardsByDeckId(sqlite3* db, int deck_id);
void freeCardList(struct Cardlist* head);

#endif //YKNAA_CARD_H
