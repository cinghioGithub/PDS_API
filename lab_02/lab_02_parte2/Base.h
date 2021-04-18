//
// Created by Enrico on 18/04/2021.
//

#ifndef LAB_02_BASE_H
#define LAB_02_BASE_H

#include <iostream>

class Base {

public:
    std::string getName() const;
    virtual int mType() const = 0;
    virtual void ls(int indent) const = 0;
};


#endif //LAB_02_BASE_H
