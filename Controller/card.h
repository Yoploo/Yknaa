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

#endif //YKNAA_CARD_H
