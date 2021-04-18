//
// Created by Enrico on 16/04/2021.
//

#include "Directory.h"
#include <string>

Directory* Directory::root = new Directory("root", nullptr);

Directory::Directory(std::string name, Directory* f) {
    //std::cout << "costruttore: " << name << std::endl;
    //if(root == nullptr) root = this;
    this->name = name;
    this->father = f;
}

Directory::~Directory() {
    if(root == this) root = nullptr;
    for (auto it = this->children.begin(); it != this->children.end(); ++it) {
        delete it->second;
    }
    //std::cout << "distruttore: " << this->name << std::endl;
}

Directory *Directory::getRoot() {
    return root;
}

Directory *Directory::addDirectory(const std::string &name) {
    Directory* d = new Directory(name, this);
    std::pair<std::string, Directory*> p = std::make_pair(name, d);
    Directory* tmp = this->children.insert(p).first->second;
    return tmp;
}

Directory *Directory::get(const std::string &name) {
    if(name.compare("..")==0) return this->father;
    return this->children.find(name)->second;
}

bool Directory::remove(const std::string &name) {
    if(this->children.find(name) == this->children.end()) return false;
    if(name.compare("..")==0 || name.compare(".")==0) return false;
    delete this->children.find(name)->second;
    this->children.erase(name);
    return true;
}

bool Directory::move(const std::string &name, Directory *target) {
    if(this->children.find(name) == this->children.end()) return false;
    if(target == nullptr) return false;
    Directory* dir = this->children.find(name)->second;
    dir->father = target;
    target->children.insert(std::make_pair(name, dir));
    this->children.erase(name);
    return true;
}

bool Directory::copy(const std::string &name, Directory *target) {
    if(this->children.find(name) == this->children.end()) return false;
    if(target == nullptr) return false;
    Directory *source = this->children.find(name)->second;
    Directory *tmp = target->addDirectory(source->name);
    recursive_copy(source, tmp);
    return true;
}

void Directory::recursive_copy(Directory *source, Directory *tmp) {
    if(source->children.size() == 0)
        return;
    for (auto it = source->children.begin(); it != source->children.end(); ++it) {
        Directory* dir = tmp->addDirectory(it->second->name);
        recursive_copy(it->second, dir);
    }
}

void Directory::ls(int indent) {
    for(int i=0; i<indent; i++)
        std::cout << " ";
    std::cout << this->name << std::endl;
    indent += this->name.length();
    for (auto it = this->children.begin(); it != this->children.end(); ++it) {
        it->second->ls(indent);
    }
}
