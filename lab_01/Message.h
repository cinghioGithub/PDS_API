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
    Message();
    Message(int n);
    Message(const Message& source);
    Message(Message&& source);
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
