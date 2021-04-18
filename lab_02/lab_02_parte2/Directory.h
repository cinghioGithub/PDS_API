//
// Created by Enrico on 16/04/2021.
//

#ifndef LAB_02_DIRECTORY_H
#define LAB_02_DIRECTORY_H

#include "Base.h"
#include "File.h"
#include <iostream>
#include <string>
#include <map>

class Directory: public Base{
    static Directory root;
    Directory* father;
    std::map<std::string, Base*> children;

    void recursive_copy(Directory* source, Directory* tmp);
    void recursive_ls(int indent, int deep);
    Directory(std::string name, Directory* f);
public:
    ~Directory();

    int mType() const;
    void ls(int indent) const;
    File* addFile(const std::string &name, uintmax_t size);

    static Directory* getRoot();
    Directory* addDirectory(const std::string &name);
    Directory* get(const std::string &name);
    bool remove(const std::string &name);
    bool move(const std::string &name, Directory *target);
    bool copy(const std::string &name, Directory *target);
    void ls(int indent);
};


#endif //LAB_02_DIRECTORY_H
