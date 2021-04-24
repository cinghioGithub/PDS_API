//
// Created by Enrico on 18/04/2021.
//

#ifndef LAB_02_BASE_H
#define LAB_02_BASE_H

#define FILE_ 0
#define DIR 1

#include <iostream>

class Base {
    std::string name;
public:
    Base(std::string name) {
        this->name = name;
    }
    virtual ~Base(){}
    std::string getName() const{
        return this->name;
    }
    virtual int mType() const = 0;
    virtual void ls(int indent) const = 0;
};


#endif //LAB_02_BASE_H
