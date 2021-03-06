#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <set>
#include "Proc.h"

void ordinaProcessiAperti(std::map<int, std::shared_ptr<Proc>> processi);
std::shared_ptr<Proc> getProcess (std::shared_ptr<Proc> process, int pid);
void ordinaProcessiApertiFigli(std::shared_ptr<Proc> process);
void ordinaFileApertiFigli(std::shared_ptr<Proc> process);
void ordinaFileMappatiFigli(std::shared_ptr<Proc> process);
void listaProcessiStatoFigli(std::shared_ptr<Proc> process);

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

    ordinaProcessiAperti(processi);

    //ordinamento file aperti
    std::map<std::string, int> orderedProcessi;
    for (auto it = processi.begin(); it != processi.end(); ++it){
        std::list<std::string> listString = it->second->getOpenFile();
        for(auto file = listString.begin(); file != listString.end(); ++file){
            if(orderedProcessi.find(*file) != orderedProcessi.end()){
                orderedProcessi.find(*file)->second = orderedProcessi.find(*file)->second + 1;
            }
            else{
                orderedProcessi.insert(std::make_pair(*file, 1));
            }
        }
    }
    std::vector<std::pair<std::string, int> > arr;
    for (const auto &item : orderedProcessi) {
        arr.emplace_back(item);
    }
    std::sort(arr.begin(), arr.end(), [] (const auto &x, const auto &y) { return x.second < y.second; });
    for(auto it = arr.begin(); it != arr.end(); ++it){
        std::cout << it->first << " aperto in " << it->second << " processi" << std::endl;
    }
    //ordinamento file mappati
    std::map<std::string, int> orderedMappedFile;
    for (auto it = processi.begin(); it != processi.end(); ++it){
        std::list<std::string> listString = it->second->getMappedFile();
        for(auto file = listString.begin(); file != listString.end(); ++file){
            if(orderedMappedFile.find(*file) != orderedMappedFile.end()){
                orderedMappedFile.find(*file)->second = orderedMappedFile.find(*file)->second + 1;
            }
            else{
                orderedMappedFile.insert(std::make_pair(*file, 1));
            }
        }
    }
    std::vector<std::pair<std::string, int> > arr1;
    for (const auto &item : orderedMappedFile) {
        arr1.emplace_back(item);
    }
    std::sort(arr1.begin(), arr1.end(), [] (const auto &x, const auto &y) { return x.second < y.second; });
    for(auto it = arr1.begin(); it != arr1.end(); ++it){
        std::cout << it->first << " utilizzato in " << it->second << " processi" << std::endl;
    }
    //ordinamento processi stato
    for (auto it = processi.begin(); it != processi.end(); ++it){
        if(it->second->getState() == "I"){
            std::cout << "Pid: " << it->second->getPid() << std::endl;
        }
    }

    std::shared_ptr<Proc> findProcess = getProcess(zero, 2);
    ordinaProcessiApertiFigli(findProcess);
    ordinaFileApertiFigli(findProcess);
    ordinaFileMappatiFigli(findProcess);
    listaProcessiStatoFigli(findProcess);

    return 0;
}

void ordinaProcessiAperti(std::map<int, std::shared_ptr<Proc>> processi) {
    std::vector<std::shared_ptr<Proc>> orderedOpenFile;
    for (auto it = processi.begin(); it != processi.end(); ++it){
        orderedOpenFile.push_back(it->second);
    }
    //ordine decrescente
    std::sort(orderedOpenFile.begin(), orderedOpenFile.end(),
              [](std::shared_ptr<Proc> a, std::shared_ptr<Proc> b) -> bool {
                  return a->getNumberOpenFile() > b->getNumberOpenFile();
              });

    for (auto it = orderedOpenFile.begin(); it != orderedOpenFile.end(); ++it){
        std::cout << "Processo: " << it->get()->getPid() << ", con " << it->get()->getNumberOpenFile() << " file aperti" << std::endl;
    }
}

std::shared_ptr<Proc> getProcess (std::shared_ptr<Proc> process, int pid){
    if(process->getPid() == pid)
        return process;

    std::shared_ptr<Proc> tmp;
    std::list<std::shared_ptr<Proc>> children = process->getChildren();
    for(auto child = children.begin(); child != children.end(); ++child){
        tmp = getProcess(*child, pid);
        if(tmp != std::shared_ptr<Proc>{})
            return tmp;
    }
    return std::shared_ptr<Proc>{};
}

void recursivePopulate(std::shared_ptr<Proc> process, std::vector<std::shared_ptr<Proc>> &orderedOpenFile){
    std::list<std::shared_ptr<Proc>> children = process->getChildren();
    for(auto child = children.begin(); child != children.end(); ++child){
        orderedOpenFile.push_back(*child);
        recursivePopulate(*child, orderedOpenFile);
    }
}

void ordinaProcessiApertiFigli(std::shared_ptr<Proc> process){
    std::vector<std::shared_ptr<Proc>> orderedOpenFile;
    recursivePopulate(process, orderedOpenFile);
    std::sort(orderedOpenFile.begin(), orderedOpenFile.end(),
              [](std::shared_ptr<Proc> a, std::shared_ptr<Proc> b) -> bool {
                  return a->getNumberOpenFile() > b->getNumberOpenFile();
              });

    int c=0;
    for (auto it = orderedOpenFile.begin(); it != orderedOpenFile.end(); ++it){
        std::cout << "Processo: " << it->get()->getPid() << ", con " << it->get()->getNumberOpenFile() << " file aperti" << std::endl;
        c++;
    }
    std::cout << c << std::endl;
}

void ordinaFileApertiFigli(std::shared_ptr<Proc> process){
    std::map<std::string, int> orderedProcessi;
    std::vector<std::shared_ptr<Proc>> processi;
    recursivePopulate(process, processi);
    for (auto it = processi.begin(); it != processi.end(); ++it){
        std::list<std::string> listString = (*it)->getOpenFile();
        for(auto file = listString.begin(); file != listString.end(); ++file){
            if(orderedProcessi.find(*file) != orderedProcessi.end()){
                orderedProcessi.find(*file)->second = orderedProcessi.find(*file)->second + 1;
            }
            else{
                orderedProcessi.insert(std::make_pair(*file, 1));
            }
        }
    }
    std::vector<std::pair<std::string, int> > arr;
    for (const auto &item : orderedProcessi) {
        arr.emplace_back(item);
    }
    std::sort(arr.begin(), arr.end(), [] (const auto &x, const auto &y) { return x.second < y.second; });
    int c=0;
    for(auto it = arr.begin(); it != arr.end(); ++it){
        std::cout << it->first << " aperto in " << it->second << " processi" << std::endl;
        c++;
    }
    std::cout << c << std::endl;;
}

void ordinaFileMappatiFigli(std::shared_ptr<Proc> process){
    std::map<std::string, int> orderedMappedFile;
    std::vector<std::shared_ptr<Proc>> processi;
    recursivePopulate(process, processi);
    for (auto it = processi.begin(); it != processi.end(); ++it){
        std::list<std::string> listString = (*it)->getMappedFile();
        for(auto file = listString.begin(); file != listString.end(); ++file){
            if(orderedMappedFile.find(*file) != orderedMappedFile.end()){
                orderedMappedFile.find(*file)->second = orderedMappedFile.find(*file)->second + 1;
            }
            else{
                orderedMappedFile.insert(std::make_pair(*file, 1));
            }
        }
    }
    std::vector<std::pair<std::string, int> > arr1;
    for (const auto &item : orderedMappedFile) {
        arr1.emplace_back(item);
    }
    std::sort(arr1.begin(), arr1.end(), [] (const auto &x, const auto &y) { return x.second < y.second; });
    int c=0;
    for(auto it = arr1.begin(); it != arr1.end(); ++it){
        std::cout << it->first << " utilizzato in " << it->second << " processi" << std::endl;
        c++;
    }
    std::cout << c << std::endl;
}

void listaProcessiStatoFigli(std::shared_ptr<Proc> process){
    std::vector<std::shared_ptr<Proc>> processi;
    recursivePopulate(process, processi);
    int c=0;
    for (auto it = processi.begin(); it != processi.end(); ++it){
        if((*it)->getState() == "I"){
            std::cout << "Pid: " << (*it)->getPid() << std::endl;
            c++;
        }
    }
    std::cout << c << std::endl;
}