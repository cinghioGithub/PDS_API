//
// Created by Enrico on 18/04/2021.
//

#ifndef LAB_02_FILE_H
#define LAB_02_FILE_H

#include "Base.h"


class File: public Base{
    uintmax_t size;
    uintmax_t date;
public:
    File(uintmax_t size, uintmax_t date, std::string name);
    uintmax_t getSize() const;
    uintmax_t getDate() const;
    int mType() const;
    void ls(int indent) const;
};


#endif //LAB_02_FILE_H
