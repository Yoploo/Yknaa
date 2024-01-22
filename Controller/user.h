//
// Created by Mathéo on 19/01/2024.
//

#ifndef YKNAA_USER_H
#define YKNAA_USER_H

struct user {
    int user_id;
    const char* nickname;
    const char* password;
    int rank;
};

int registerUser(sqlite3* db, const struct user *User);
int UserExists(sqlite3* db, const char* nickname);
int loginUser(sqlite3* db, const struct user *User);
int deckByUser(sqlite3* db, const struct user *User);
int verifpassword(const char * password1, const char * password2);

#endif //YKNAA_USER_H
