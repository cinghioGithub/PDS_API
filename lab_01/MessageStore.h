//
// Created by Enrico on 28/03/2021.
//

#ifndef LAB_01_MESSAGESTORE_H
#define LAB_01_MESSAGESTORE_H
#include "Message.h"
#include <iostream>
#include <optional>

#define N 20


class MessageStore {
    Message *messages;
    int dim; // dimensione corrente array
    int n; // incremento memoria

public:
    MessageStore(int n);
    ~MessageStore();

    // inserisce un nuovo messaggio o sovrascrive quello precedente
    //se ce n’è uno presente con lo stesso id
    void add(Message &m);

    // restituisce un messaggio se presente
    std::optional<Message> get(long id);

    // cancella un messaggio se presente
    bool remove(long id);

    // restituisce il numero di messaggi validi e di elementi vuoti
    // ancora disponibili
    std::tuple<int, int> stats();

    // compatta l’array (elimina le celle vuole e riporta l’array
    // alla dimensione multiplo di n minima in grado di contenere
    // tutte le celle
    void compact();

    int getDim() const;
};


#endif //LAB_01_MESSAGESTORE_H
