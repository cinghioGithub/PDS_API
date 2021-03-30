//
// Created by Enrico on 28/03/2021.
//

#include "MessageStore.h"
#include <cstring>
#include <tuple>

MessageStore::MessageStore(int n) {
    this->messages = new Message[n];
    this->dim = n;
    this->n = 10;
}

MessageStore::~MessageStore(){
    delete[] this->messages;
}

// inserisce un nuovo messaggio o sovrascrive quello precedente
//se ce n’è uno presente con lo stesso id
void MessageStore::add(Message &m){
    if(m.getId()!=-1) {
        for (int i = 0; i < this->dim; i++) {
            if (m.getId() == this->messages[i].getId()) {
                this->messages[i] = m;
                return;
            }
        }
        for (int i = 0; i < this->dim; i++) {
            if (this->messages[i].getId() == -1) {
                this->messages[i] = m;
                return;
            }
        }
    }
    Message *tmp = new Message[this->dim+this->n];
    //memcpy(tmp, this->messages, this->dim*sizeof(Message));
    for(int i=0; i<this->dim; i++)
        tmp[i] = std::move(this->messages[i]);   //l'assegnazione per movimento è più veloce dell'assegnazione per copia

    //delete[] this->messages;   //non necessaria dato che l'assegnazione è per movimento
    this->messages = tmp;
    this->messages[dim] = m;
    this->dim += n;
}

// restituisce un messaggio se presente
std::optional<Message> MessageStore::get(long id){
    for(int i=0; i<this->dim; i++){
        if(this->messages[i].getId() == id)
            return this->messages[i];
    }
    return {};
}

// cancella un messaggio se presente
bool MessageStore::remove(long id){
    for(int i=0; i<this->dim; i++){
        if(this->messages[i].getId() == id){
            this->messages[i] = Message();
            return true;
        }
    }
    return false;
}

// restituisce il numero di messaggi validi e di elementi vuoti
// ancora disponibili
std::tuple<int, int> MessageStore::stats(){
    int validi=0, disponibili=0;
    for(int i=0; i<this->dim; i++){
        if(this->messages[i].getId()==-1)
            disponibili++;
        else
            validi++;
    }
    return std::make_tuple(validi, disponibili);
}

// compatta l’array (elimina le celle vuole e riporta l’array
// alla dimensione multiplo di n minima in grado di contenere
// tutte le celle
void MessageStore::compact(){
    Message *tmp = NULL;
    int validi = std::get<0>(this->stats());
    int newDim=0, j=0;

    while(newDim < validi)
        newDim += n;

    tmp = new Message[newDim];
    for(int i=0; i< this->dim; i++){
        if(this->messages[i].getId() != -1) {
            tmp[j] = this->messages[i];
            j++;
        }
    }
    delete[] this->messages;
    this->messages = tmp;
    this->dim = newDim;
}
