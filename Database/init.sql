CREATE TABLE IF NOT EXISTS users (
                                     id INTEGER PRIMARY KEY,
                                     nickname TEXT NOT NULL,
                                     password TEXT NOT NULL,
                                     rank INTEGER
);

CREATE TABLE IF NOT EXISTS decks (
                                     deck_id INTEGER PRIMARY KEY,
                                     subject TEXT NOT NULL,
                                     description TEXT NOT NULL,
                                     tag TEXT,
                                     status TEXT,
                                     user_id INTEGER,
                                     FOREIGN KEY (user_id) REFERENCES users(id)
    );

CREATE TABLE IF NOT EXISTS cards (
                                     card_id INTEGER PRIMARY KEY,
                                     recto TEXT NOT NULL,
                                     verso TEXT NOT NULL,
                                     rank INTEGER,
                                     points INTEGER,
                                     deck_id INTEGER,
                                     user_id INTEGER,
                                     FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (deck_id) REFERENCES decks(deck_id)
    );