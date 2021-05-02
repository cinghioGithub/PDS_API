//
// Created by Enrico on 16/04/2021.
//

#include "Directory.h"
#include <memory>
#include <string>

std::shared_ptr<Directory> Directory::root{
    new Directory{
        "root",
        std::shared_ptr<Directory>{}
    }
};

int Directory::mType() const {
    return DIR;
}
void Directory::ls(int indent) const { }

Directory::Directory(std::string name, std::weak_ptr<Directory> f): Base(name) {
    //std::cout << "costruttore: " << name << std::endl;
    //if(root == nullptr) root = this;
    this->father = f;
    //this->self = shared_from_this();
}

std::weak_ptr<Directory> Directory::getSelf() {
    return this->self;
}

Directory::~Directory() {
    std::cout << "distruttore: " << this->getName() << std::endl;
}

std::shared_ptr<Directory> Directory::getRoot() {
    return root;
}

std::shared_ptr<Directory> Directory::addDirectory(const std::string &name) {
    std::shared_ptr<Base> tmp;
    if(this->children.find(name) == this->children.end()){
        std::shared_ptr<Directory> d{ new Directory{name, this->getSelf()}};
        std::pair<std::string, std::shared_ptr<Base>> p = std::make_pair(name, d);
        tmp = this->children.insert(p).first->second;
    }
    else{
        tmp = this->children.find(name)->second;
        if(tmp->mType() == FILE_) tmp = std::shared_ptr<Directory>{};
    }
    return std::static_pointer_cast<Directory>(tmp);
}

std::weak_ptr<Base> Directory::get(const std::string &name) {
    //if(name=="..") return this->father;
    return this->children.find(name)->second;
}

bool Directory::remove(const std::string &name) {
    if(this->children.find(name) == this->children.end())
        return false;
    if(name==".." || name==".")
        return false;
    //delete this->children.find(name)->second;
    this->children.erase(name);
    return true;
}

bool Directory::move(const std::string &name, std::shared_ptr<Directory> target) {
    if(this->children.find(name) == this->children.end())
        return false;
    if(target == std::shared_ptr<Directory>{})
        return false;
    if(this->children.find(name)->second->mType()==DIR){
        std::shared_ptr<Directory> dir = static_pointer_cast<Directory>(this->children.find(name)->second);
        dir->father = target;
        target->children.insert(std::make_pair(name, dir));
        this->children.erase(name);
        return true;
    }
    return false;
}

bool Directory::copy(const std::string &name, std::shared_ptr<Directory> target) {
    if(this->children.find(name) == this->children.end()) return false;
    if(target == std::shared_ptr<Directory>{}) return false;
    std::shared_ptr<Directory> source = static_pointer_cast<Directory>(this->children.find(name)->second);
    std::shared_ptr<Directory> tmp = target->addDirectory(source->getName());
    recursive_copy(source, tmp);
    return true;
}

void Directory::recursive_copy(std::shared_ptr<Directory> source, std::shared_ptr<Directory> tmp) {
    if(source->children.size() == 0)
        return;
    for (auto it = source->children.begin(); it != source->children.end(); ++it) {
        std::shared_ptr<Directory> dir = tmp->addDirectory(it->second->getName());
        recursive_copy(static_pointer_cast<Directory>(it->second), dir);
    }
}

void Directory::ls(int indent) {
    this->recursive_ls(indent, 0);
}

void Directory::recursive_ls(int indent, int deep) {
    for(int i=0; i<indent*deep; i++)
        std::cout << " ";
    std::cout << this->getName() << std::endl;
    for (auto it = this->children.begin(); it != this->children.end(); ++it) {
        if(it->second->mType()==DIR)
            (static_pointer_cast<Directory>(it->second))->recursive_ls(indent, deep+1);
        else{
            it->second->ls(indent*(deep+1));
        }
    }
}

std::shared_ptr<File> Directory::addFile(const std::string &name, uintmax_t size) {
    std::shared_ptr<File> tmp = std::shared_ptr<File>{};
    if(this->children.find(name) == this->children.end()){
        tmp = std::make_shared<File>( size, 0, name);
        this->children.insert(std::make_pair(name, tmp));
    }
    return tmp;
}

std::shared_ptr<Directory> Directory::getDirectory(const std::string &name) {
    //if(name.compare("..")==0) return this->father;
    if(this->children.find(name)->second->mType()==DIR){
        return static_pointer_cast<Directory>(this->children.find(name)->second);
    }
    return std::shared_ptr<Directory>{};
}

std::shared_ptr<File> Directory::getFile(const std::string &name) {
    std::shared_ptr<File> tmp = std::shared_ptr<File>{};
    if(this->children.find(name)->second->mType() == FILE_)
        tmp = static_pointer_cast<File>(this->children.find(name)->second);
    return tmp;
}
