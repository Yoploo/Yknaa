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

#endif //YKNAA_DECK_H
