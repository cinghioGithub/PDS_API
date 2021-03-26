//
// Created by Enrico on 24/03/2021.
//

#include <iostream>
#include <cstring>
#include "Message.h"

long Message::messageNumber = 0;

//costruttore di default
Message::Message(): id(-1), size(0), data((char *)"") {
    std::cout << "Message::defualt constructor" << std::endl;
}

//coustruttore
Message::Message(int n): size(n) {
    std::cout << "Message::constructor" << std::endl;
    this->id = messageNumber;
    messageNumber++;
    this->data = mkMessage(this->size);
}

//costruttore di copia
Message::Message(const Message& source) {
    std::cout << "Message::copy constructor" << std::endl;
    this->size = source.size;
    this->id = source.id;
    this->data = new char[source.size + 1];
    /*for(int i=0; i<source.size; i++)
        this->data[i] = source.data[i];
    this->data[this->size] = '\0';*/
    memcpy(this->data, source.data, this->size+1);
}

//costruttore di movimento
Message::Message(Message&& source) {
    std::cout << "Message::movement constructor" << std::endl;
    this->size = source.size;
    this->id = source.id;
    this->data = source.data;
    source.data = nullptr;
}

//distruttore
Message::~Message(){
    std::cout << "Message::distructor" << std::endl;
    delete[] this->data;
}

long Message::getId() const{ return this->id; }
int Message::getSize() const{ return this->size; }
char* Message::getData() const{ return this->data; }

char* Message::mkMessage(int n){
    std::string vowels = "aeiou";
    std::string consonants = "bcdfghlmnpqrstvz";
    char* m = new char[n+1];
    for(int i=0; i<n; i++){
        m[i]= i%2 ? vowels[rand()%vowels.size()] :
              consonants[rand()%consonants.size()];
    }
    m[n] = 0 ;
    return m;
}

std::ostream& operator<<(std::ostream& out, const Message& m){
// accedere alle parti contenute nell’oggetto Message e
// stamparle utilizzando out << … ;
// Questa funzione DEVE restituire l’oggetto out

    out << "[id:" << m.getId() << "]" << "[size:" << m.getSize() << "]" << "[data:" << m.getData() << "]";
    return out;
}
