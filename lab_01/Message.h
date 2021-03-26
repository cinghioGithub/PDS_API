//
// Created by Enrico on 24/03/2021.
//

#ifndef LAB_01_MESSAGE_H
#define LAB_01_MESSAGE_H

#include <string>

class Message {
    long id;
    char *data;
    int size;

public:
    //costruttore di dafault
    Message();
    //costruttore
    Message(int n);
    //costruttore di copia
    Message(const Message& source);
    //costruttore di movimento
    Message(Message&& source);
    //opearatore di assegnazione
    Message& operator=(const Message& source);
    //opearatore di assegnazione per movimento
    Message& operator=(Message&& source);
    //distruttore
    ~Message();

    long getId() const;
    int getSize() const;
    char* getData() const;

private:
    static long messageNumber;
    char* mkMessage(int n);

};

std::ostream& operator<<(std::ostream& out, const Message& m);


#endif //LAB_01_MESSAGE_H
