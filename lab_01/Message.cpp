//
// Created by Enrico on 24/03/2021.
//

#include <iostream>
#include <cstring>
#include "Message.h"

long Message::messageNumber = 0;
long Message::allocazione = 0;

//costruttore di default
Message::Message(): id(-1), size(0) {
    //std::cout << "Message::defualt constructor" << std::endl;
    allocazione++;
    messageNumber++;
    this->data = mkMessage(0);
}

//coustruttore
Message::Message(int n): size(n) {
    //std::cout << "Message::constructor" << std::endl;
    allocazione++;
    this->id = messageNumber;
    messageNumber++;
    this->data = mkMessage(this->size);
}

//costruttore di copia
Message::Message(const Message& source) {
    //std::cout << "Message::copy constructor" << std::endl;
    allocazione++;
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
    //std::cout << "Message::movement constructor" << std::endl;
    allocazione++;
    this->size = source.size;
    this->id = source.id;
    this->data = source.data;
    source.data = NULL;
}

//operatore di asseganzione
Message& Message::operator=(const Message& source){
    //std::cout << "Message::assegnation operator" << std::endl;
    if(this!=&source){
        delete[] this->data;
        this->data = NULL;
        this->size = source.size;
        this->id = source.id;
        this->data = new char[source.size + 1];
        memcpy(this->data, source.data, this->size + 1);
    }
    return *this;
}

//opearatore di assegnazione per movimento
Message& Message::operator=(Message&& source){
    //std::cout << "Message::assegnation operator (movement)" << std::endl;
    if(this!=&source){
        delete[] this->data;
        this->data = source.data;
        this->size = source.size;
        this->id = source.id;
        source.data = NULL;
    }
    return *this;
}

//distruttore
Message::~Message(){
    //std::cout << "Message::distructor" << " " << this->id << std::endl;
    allocazione--;
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
