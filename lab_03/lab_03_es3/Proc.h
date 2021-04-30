//
// Created by Enrico on 30/04/2021.
//

#ifndef LAB_03_ES3_PROC_H
#define LAB_03_ES3_PROC_H

#include <iostream>
#include <map>
#include <list>

class Proc {
    std::map<std::string,std::string> status;
    int exeTime;
    std::list<std::string> openFile;
    std::list<std::string> mappedFile;
public:
    Proc(std::map<std::string,std::string> status, std::list<std::string> open, std::list<std::string> mapped, int time);
    ~Proc();
    int getPid();
};


#endif //LAB_03_ES3_PROC_H
