//
// Created by Enrico on 30/04/2021.
//

#include "Proc.h"

Proc::Proc(int pid, std::map<std::string,std::string> status, std::list<std::string> open, std::list<std::string> mapped, std::string stat){
    this->pid = pid;
    this->status = status;
    this->openFile = open;
    this->mappedFile = mapped;
    this->stat = stat;
    //std::cout << "Oggetto proc costruito: " << this->pid << std::endl;
}

Proc::~Proc() {
    //std::cout << "Oggetto proc distrutto: " << this->pid << std::endl;
}

int Proc::getPid() {
    return this->pid;
}

void Proc::addChild(std::shared_ptr<Proc> child) {
    this->children.push_back(child);
}

void Proc::setFather(std::shared_ptr<Proc> father) {
    this->father = father;
}

int Proc::getPPid() {
    if(this->status.find("PPid") != this->status.end()){
        std::string tmp = this->status.find("PPid")->second;
        int ppid = std::stoi(tmp);
        return std::stoi(tmp);
    }
    return 0;
}

int Proc::getNumberOpenFile() {
    return this->openFile.size();
}

std::list<std::string> Proc::getOpenFile() {
    return this->openFile;
}

std::list<std::string> Proc::getMappedFile() {
    return this->mappedFile;
}

std::string Proc::getState() {
    if(this->status.find("State") != this->status.end()){
        return this->status.find("State")->second.substr(0, 1);
    }
    return std::string{};
}
