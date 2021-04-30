//
// Created by Enrico on 30/04/2021.
//

#include "Proc.h"

Proc::Proc(std::map<std::string,std::string> status, std::list<std::string> open, std::list<std::string> mapped, int time){
    this->status = status;
    this->openFile = open;
    this->mappedFile = mapped;
    this->exeTime = time;
    std::cout << "Oggetto proc costruito" << std::endl;
}

Proc::~Proc() {
    std::cout << "Oggetto proc distrutto" << std::endl;
}

int Proc::getPid() {
    return std::stoi(this->status.find("Pid")->second);
}
