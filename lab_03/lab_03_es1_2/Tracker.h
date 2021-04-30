//
// Created by Enrico on 30/04/2021.
//

#ifndef LAB_03_TRACKER_H
#define LAB_03_TRACKER_H

#include <iostream>
#include <cstdio>

template <typename T>
class Tracker {
    static u_int count;
    friend T;
public:
    Tracker(){count++;}
    ~Tracker(){count--;}
};

template<typename T>
u_int Tracker<T>::count = 0;

#endif //LAB_03_TRACKER_H
