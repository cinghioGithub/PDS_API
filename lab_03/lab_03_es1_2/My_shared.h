//
// Created by Enrico on 30/04/2021.
//

#ifndef LAB_03_MY_SHARED_H
#define LAB_03_MY_SHARED_H

template <class T>
class My_shared{
    T* ref;
    int* counter;
public:
    My_shared(T* val){
        this->ref = val;
        this->counter = new int(1);
    }
    My_shared(const My_shared<T> &sp){
        this->ref = sp.ref;
        this->counter = sp.counter;
        (*this->counter)++;
    }
    ~My_shared(){
        if(this->ref!= nullptr && this->counter!= nullptr) {
            (*this->counter)--;
            if ((*this->counter) == 0) {
                delete this->counter;
                delete this->ref;
            }
        }
    }
    T* operator->(){
        return this->ref;
    }
    T& operator*(){
        return (*this->ref);
    }
    //assegnazione per copia
    My_shared<T> &operator=(const My_shared<T> &other){
        if(this != &other){
            (*this->counter)--;
            if((*this->counter)==0){
                delete this->ref;
                this->ref = nullptr;
                delete this->counter;
                this->counter = nullptr;
            }
            this->ref = other.ref;
            this->counter = other.counter;
            *(this->counter)++;
        }
        return *this;
    }
    //assegnazione per movimento
    My_shared<T> &operator=(My_shared<T> &&other){
        if(this != &other){
            (*this->counter)--;
            if((*this->counter)==0){
                delete this->ref;
                this->ref = nullptr;
                delete this->counter;
                this->counter = nullptr;
            }
            this->ref = other.ref;
            this->counter = other.counter;
            other.ref = nullptr;
            other.counter = nullptr;
        }
        return *this;
    }
    uint use_count(){
        return (*this->counter);
    }
};

#endif //LAB_03_MY_SHARED_H
