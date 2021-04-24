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
#include <memory>

class Directory: public Base{
    static std::shared_ptr<Directory> root;
    std::weak_ptr<Directory> father;
    std::weak_ptr<Directory> self;
    std::map<std::string, std::shared_ptr<Base>> children;

    //void recursive_copy(Directory* source, Directory* tmp);
    void recursive_ls(int indent, int deep) const;
    Directory(std::string name, std::shared_ptr<Directory> f);
public:
    ~Directory() override;

    int mType() const override;
    void ls(int indent) const override;
    //File* addFile(const std::string &name, uintmax_t size);
    //Directory* getDirectory(const std::string &name);
    //File* getFile(const std::string &name);

    static std::shared_ptr<Directory> getRoot();
    std::shared_ptr<Directory> addDirectory(const std::string &name);
    std::shared_ptr<Base> get(const std::string &name);
    bool remove(const std::string &name);
    bool move(const std::string &name, std::shared_ptr<Directory>);
    //bool copy(const std::string &name, Directory *target);
    //void ls(int indent);
};


#endif //LAB_02_DIRECTORY_H
