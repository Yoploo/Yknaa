//
// Created by loloy on 18/01/2024.
//

#ifndef YKNAA_DECK_H
#define YKNAA_DECK_H

struct deck {
    const char* subject;
    const char* description;
    const char* tag;
    const char* status;
    int user_id;
};

struct decklist{
    int deck_id;
    const char* subject;
    const char* description;
    const char* tag;
    const char* status;

    struct decklist* next;
};


int addDeck(sqlite3* db, const struct deck* Deck);
int deleteDeck(sqlite3* db, const struct deck* Deck, int deck_id);
struct decklist* addDeckToList(struct decklist* head, int deck_id, const char* subject, const char* description, const char* tag, const char* status);
struct decklist* decksByUserId(sqlite3* db, int user_id);
void freeDeckList(struct decklist* deckList);
int editDeck(sqlite3* db, int deck_id, int user_id);

#endif //YKNAA_DECK_H
