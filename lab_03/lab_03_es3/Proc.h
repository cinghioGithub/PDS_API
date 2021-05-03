//
// Created by Enrico on 30/04/2021.
//

#ifndef LAB_03_ES3_PROC_H
#define LAB_03_ES3_PROC_H

#include <iostream>
#include <memory>
#include <map>
#include <list>

class Proc {
    std::map<std::string,std::string> status;
    std::string stat;
    std::list<std::string> openFile;
    std::list<std::string> mappedFile;
    std::list<std::shared_ptr<Proc>> children;
    std::weak_ptr<Proc> father;
    int pid;
public:
    Proc(int pid, std::map<std::string,std::string> status, std::list<std::string> open, std::list<std::string> mapped, std::string stat);
    ~Proc();
    int getPid();
    int getPPid();
    void addChild(std::shared_ptr<Proc> child);
    void setFather(std::shared_ptr<Proc> father);
    int getNumberOpenFile();
};


#endif //LAB_03_ES3_PROC_H
