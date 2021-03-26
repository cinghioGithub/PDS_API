//
// Created by Enrico on 26/03/2021.
//

#include "DurationLogger.h"

DurationLogger::DurationLogger(const std::string& name): name(name){
    this->inizio = clock();
    std::cout << ">>>>>>>>>>>>>> starting [" << name << "]" << std::endl;
}
DurationLogger::~DurationLogger(){
    std::cout << "<<<<<<<<<<<<<<< ending [" << this->name << "]: [" <<  (float)(clock()-this->inizio)/CLOCKS_PER_SEC << "] sec" << std::endl;
}
