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

int verifpassword(const char * password1, const char * password2);
int registerUser(sqlite3* db, const struct user *User);

#endif //YKNAA_USER_H
