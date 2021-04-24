//
// Created by Enrico on 16/04/2021.
//

#include "Directory.h"
#include <string>

std::shared_ptr<Directory> Directory::root = std::shared_ptr<Directory>(new Directory("root", nullptr));

int Directory::mType() const {
    return DIR;
}
//void Directory::ls(int indent) const { }

Directory::Directory(std::string name, std::shared_ptr<Directory> f): Base(name) {
    //std::cout << "costruttore: " << name << std::endl;
    //if(root == nullptr) root = this;
    this->father = f;
}

Directory::~Directory() {
    //for (auto it = this->children.begin(); it != this->children.end(); ++it) {
    //    delete it->second;
    //}
    //std::cout << "distruttore: " << this->name << std::endl;
}

std::shared_ptr<Directory> Directory::getRoot() {
    return root;
}

std::shared_ptr<Directory> Directory::addDirectory(const std::string &name) {
    //std::cout << this->getName() << std::endl;
    std::shared_ptr<Base> tmp;
    if(this->children.find(name) == this->children.end()){
        std::shared_ptr<Directory> d(new Directory(name, (std::shared_ptr<Directory>)this));
        std::pair<std::string, std::shared_ptr<Base>> p = std::make_pair(name, d);
        tmp = this->children.insert(p).first->second;
    }
    else{
        tmp = this->children.find(name)->second;
        if(tmp->mType() == FILE_) tmp = nullptr;
    }
    return static_pointer_cast<Directory>(tmp);
}

std::shared_ptr<Base> Directory::get(const std::string &name) {
    std::shared_ptr<Base> tmp = nullptr;
    if(name.compare("..")==0) {
        tmp = this->father.lock();
    }
    else
        tmp = this->children.find(name)->second;
    return tmp;
}

bool Directory::remove(const std::string &name) {
    if(this->children.find(name) == this->children.end())
        return false;
    if(name==".." || name==".")
        return false;
    //delete this->children.find(name)->second;
    this->children.find(name)->second.reset();
    this->children.erase(name);
    return true;
}

bool Directory::move(const std::string &name, std::shared_ptr<Directory> target) {
    if(this->children.find(name) == this->children.end())
        return false;
    if(target == nullptr)
        return false;
    if(this->children.find(name)->second->mType()==DIR){
        std::shared_ptr<Directory> dir = static_pointer_cast<Directory>(this->children.find(name)->second);
        dir->father = target;
        target->children.insert(std::make_pair(name, dir));
        this->children.find(name)->second.reset();
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

/*TODO*/

void Directory::ls(int indent) const {
    this->recursive_ls(indent, 0);
}

void Directory::recursive_ls(int indent, int deep) const {
    for(int i=0; i<indent*deep; i++)
        std::cout << " ";
    std::cout << this->getName() << std::endl;
    for (auto it = this->children.begin(); it != this->children.end(); ++it) {
        if(it->second->mType()==DIR)
            (static_pointer_cast<Directory>(it->second))->recursive_ls(indent, deep+1);
        else{
            for(int i=0; i<indent*(deep+1); i++)
                std::cout << " ";
            std::cout << it->second->getName() << std::endl;
        }
    }
}
/*
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
*/