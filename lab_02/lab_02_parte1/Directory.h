//
// Created by Enrico on 16/04/2021.
//

#ifndef LAB_02_DIRECTORY_H
#define LAB_02_DIRECTORY_H

#include <iostream>
#include <string>
#include <map>

class Directory {
    static Directory* root;
    std::string name;
    Directory* father;
    std::map<std::string, Directory*> children;

    void recursive_copy(Directory* source, Directory* tmp);
public:
    Directory(std::string name, Directory* f);
    ~Directory();

    static Directory* getRoot();
    Directory* addDirectory(const std::string &name);
    Directory* get(const std::string &name);
    bool remove(const std::string &name);
    bool move(const std::string &name, Directory *target);
    bool copy(const std::string &name, Directory *target);
    void ls(int indent);
};


#endif //LAB_02_DIRECTORY_H
