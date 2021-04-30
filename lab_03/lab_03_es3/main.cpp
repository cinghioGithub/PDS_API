#include <iostream>
#include <fstream>
#include <memory>
#include "Proc.h"

int main() {
    std::map<int, std::shared_ptr<Proc>> processi;
    char line[500];
    std::string letto;
    std::ifstream fin;
    fin.open("..\\proc_data\\proc_data1.txt", std::ios::in);

    fin.getline(line, 500);  // "----"

    while(fin.good()){
        letto = std::string(line);
        // lettura nome processo
        fin.getline(line, 500);    // "/proc/n"
        letto = std::string(line);
        std::string pid = letto.substr(6);
        std::cout << pid << std::endl;
        fin.getline(line, 500);   // "----"
        // /proc/n/status
        fin.getline(line, 500);  // "/proc/n/status"
        do{
            fin.getline(line, 500);
            letto = std::string(line);
        }while(letto!="----");
        // /proc/n/stat
        fin.getline(line, 500);  // "/proc/n/stat"
        do{
            fin.getline(line, 500);
            letto = std::string(line);
        }while(letto!="----");
        // /proc/n/fd
        fin.getline(line, 500);  // "/proc/n/fd"
        do{
            fin.getline(line, 500);
            letto = std::string(line);
        }while(letto!="----");
        // /proc/n/map_files
        fin.getline(line, 500);  // "/proc/n/map_files"
        do{
            fin.getline(line, 500);
            letto = std::string(line);
        }while(letto!="----" && fin.good());
    }
    fin.close();
    return 0;
}
