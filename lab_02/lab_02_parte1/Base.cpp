//
// Created by Enrico on 18/04/2021.
//

#include "Base.h"

int Base::count = 0;

Base::Base(){
    count++;
}

Base::~Base() {
    count--;
    if(count==0)
        std::cout << count << std::endl;
}
