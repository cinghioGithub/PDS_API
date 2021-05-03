#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include "Proc.h"

int main() {
    std::map<int, std::shared_ptr<Proc>> processi;
    std::shared_ptr<Proc> zero{ new Proc{0, std::map<std::string,std::string>{}, std::list<std::string>{}, std::list<std::string>{}, ""}};
    processi.insert(std::make_pair(0, zero));
    char line[500];
    std::string letto;
    std::ifstream fin;

    //primo ciclo per creare tutti i processi
    fin.open("..\\proc_data\\proc_data1.txt", std::ios::in);
    fin.getline(line, 500);  // "----"

    while(fin.good()){
        int pid;
        letto = std::string(line);
        // lettura nome processo
        fin.getline(line, 500);    // "/proc/n"
        letto = std::string(line);
        std::string pid_str = letto.substr(6);
        pid = std::stoi(pid_str);

        fin.getline(line, 500);   // "----"
        // /proc/n/status
        fin.getline(line, 500);  // "/proc/n/status"
        std::map<std::string,std::string> status;
        do{
            fin.getline(line, 500);
            letto = std::string(line);
            if(letto != "----"){
                std::string key = letto.substr(0,letto.find(":\t"));
                std::string value = letto.substr(letto.find(":\t")+2, letto.length());
                /*if(key == "PPid"){
                    father = processi.find(std::stoi(value.substr(value.find("\t")+1, value.length())))->second;
                }*/
                status.insert(std::make_pair(key, value));
            }
        }while(letto!="----");
        // /proc/n/stat
        fin.getline(line, 500);  // "/proc/n/stat"
        std::string stat;
        do{
            fin.getline(line, 500);
            letto = std::string(line);
            if(letto != "----")
                stat = letto;
        }while(letto!="----");
        // /proc/n/fd
        fin.getline(line, 500);  // "/proc/n/fd"
        fin.getline(line, 500);   // "total 0"
        fin.getline(line, 500);   // "dr-x------ 2 root root 0 Apr 27 22:03 ."
        fin.getline(line, 500);   // "dr-x------ 2 root root 0 Apr 27 22:03 .."
        std::list<std::string> open;
        do{
            fin.getline(line, 500);
            letto = std::string(line);
            if(letto != "----")
                open.push_back(letto.substr(letto.find("-> ")+3, letto.length()));
        }while(letto!="----");
        // /proc/n/map_files
        fin.getline(line, 500);  // "/proc/n/map_files"
        fin.getline(line, 500);   // "total 0"
        fin.getline(line, 500);   // "dr-x------ 2 root root 0 Apr 27 22:03 ."
        fin.getline(line, 500);   // "dr-x------ 2 root root 0 Apr 27 22:03 .."
        std::list<std::string> map;
        do{
            fin.getline(line, 500);
            letto = std::string(line);
            if(letto != "----" && letto != "")
                map.push_back(letto.substr(letto.find("-> ")+3, letto.length()));
        }while(letto!="----" && fin.good());
        std::shared_ptr<Proc> p{ new Proc{pid, status, open, map, stat} };
        //father->addChild(p);
        //p->setFather(father);
        processi.insert(std::make_pair(pid, p));
    }
    fin.close();

    //secondo ciclo per aggiungere i figli
    for (auto it = processi.begin(); it != processi.end(); ++it){
        if(it->second->getPid() != 0){
            std::shared_ptr<Proc> father = processi.find(it->second->getPPid())->second;
            father->addChild(it->second);
            it->second->setFather(father);
        }
    }

    std::vector<std::shared_ptr<Proc>> ordered;
    for (auto it = processi.begin(); it != processi.end(); ++it){
        ordered.push_back(it->second);
    }
    std::sort(ordered.begin(), ordered.end(), [](std::shared_ptr<Proc> a, std::shared_ptr<Proc> b) -> bool { return a->getNumberOpenFile() > b->getNumberOpenFile();});

    for (auto it = ordered.begin(); it != ordered.end(); ++it){
        std::cout << it->get()->getNumberOpenFile() << std::endl;
    }

    return 0;
}
