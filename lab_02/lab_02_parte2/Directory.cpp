//
// Created by Enrico on 16/04/2021.
//

#include "Directory.h"
#include <string>

Directory Directory::root("root", nullptr);

int Directory::mType() const {
    return DIR;
}
void Directory::ls(int indent) const { }

Directory::Directory(std::string name, Directory* f): Base(name) {
    //std::cout << "costruttore: " << name << std::endl;
    //if(root == nullptr) root = this;
    this->father = f;
}

Directory::~Directory() {
    for (auto it = this->children.begin(); it != this->children.end(); ++it) {
        delete it->second;
    }
    //std::cout << "distruttore: " << this->name << std::endl;
}

Directory *Directory::getRoot() {
    return &root;
}

Directory *Directory::addDirectory(const std::string &name) {
    Base* tmp;
    if(this->children.find(name) == this->children.end()){
        Directory* d = new Directory(name, this);
        std::pair<std::string, Directory*> p = std::make_pair(name, d);
        tmp = this->children.insert(p).first->second;
    }
    else{
        tmp = this->children.find(name)->second;
        if(tmp->mType() == FILE_) tmp = nullptr;
    }
    return (Directory*) tmp;
}

Base *Directory::get(const std::string &name) {
    if(name.compare("..")==0) return this->father;
    return this->children.find(name)->second;
}

bool Directory::remove(const std::string &name) {
    if(this->children.find(name) == this->children.end())
        return false;
    if(name.compare("..")==0 || name.compare(".")==0)
        return false;
    delete this->children.find(name)->second;
    this->children.erase(name);
    return true;
}

bool Directory::move(const std::string &name, Directory *target) {
    if(this->children.find(name) == this->children.end())
        return false;
    if(target == nullptr)
        return false;
    if(this->children.find(name)->second->mType()==DIR){
        Directory* dir = (Directory *)this->children.find(name)->second;
        dir->father = target;
        target->children.insert(std::make_pair(name, dir));
        this->children.erase(name);
        return true;
    }
    return false;
}

/*
bool Directory::copy(const std::string &name, Directory *target) {
    if(this->children.find(name) == this->children.end()) return false;
    if(target == nullptr) return false;
    Directory *source = this->children.find(name)->second;
    Directory *tmp = target->addDirectory(source->name);
    recursive_copy(source, tmp);
    return true;
}
 */

/*
void Directory::recursive_copy(Directory *source, Directory *tmp) {
    if(source->children.size() == 0)
        return;
    for (auto it = source->children.begin(); it != source->children.end(); ++it) {
        Directory* dir = tmp->addDirectory(it->second->name);
        recursive_copy(it->second, dir);
    }
}
*/

void Directory::ls(int indent) {
    this->recursive_ls(indent, 0);
}

void Directory::recursive_ls(int indent, int deep) {
    for(int i=0; i<indent*deep; i++)
        std::cout << " ";
    std::cout << this->getName() << std::endl;
    for (auto it = this->children.begin(); it != this->children.end(); ++it) {
        if(it->second->mType()==DIR)
            ((Directory *)(it->second))->recursive_ls(indent, deep+1);
        else{
            for(int i=0; i<indent*(deep+1); i++)
                std::cout << " ";
            std::cout << it->second->getName() << std::endl;
        }
    }
}

File *Directory::addFile(const std::string &name, uintmax_t size) {
    File* tmp = nullptr;
    if(this->children.find(name) == this->children.end()){
        tmp = new File(size, 0, name);
        this->children.insert(std::make_pair(name, (Base *)tmp));
    }
    return tmp;
}

Directory *Directory::getDirectory(const std::string &name) {
    if(name.compare("..")==0) return this->father;
    if(this->children.find(name)->second->mType()==DIR)
        return (Directory *)this->children.find(name)->second;
    return nullptr;
}

File *Directory::getFile(const std::string &name) {
    File* tmp = nullptr;
    if(this->children.find(name)->second->mType() == FILE_)
        tmp = (File *)this->children.find(name)->second;
    return tmp;
}
