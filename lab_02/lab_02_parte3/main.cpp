#include <iostream>
#include "Directory.h"

int main() {

    std::shared_ptr<Directory> root = Directory::getRoot();
    std::shared_ptr<Directory> alfa = root->addDirectory("alfa");
    std::cout << alfa->getName() << std::endl;
    root->remove("alfa");
    std::shared_ptr<Directory> beta = root->addDirectory("beta");
    beta->addDirectory("gamma");
    beta->move("gamma", root);
    /*
     root
        beta
        gamma
     */
    root->copy("gamma", beta);
    root->ls(4);
    beta->addFile("file", 8);
    root->ls(4);
    std::shared_ptr<Directory> dir = root->getDirectory("gamma");
    std::cout << dir->getName() << std::endl;
    std::shared_ptr<File> file = beta->getFile("file");
    std::cout << file->getName() << std::endl;
    //std::weak_ptr<Base> father = alfa->get("..");
    //std::cout << father.lock()->getName() << std::endl;

    return 0;
}
