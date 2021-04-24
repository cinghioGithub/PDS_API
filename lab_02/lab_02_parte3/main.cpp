#include <iostream>
#include "Directory.h"

int main() {

    std::shared_ptr<Directory> root = Directory::getRoot();
    root->addDirectory("alfa");
    root->getName();
    root->addDirectory("beta");
    //std::shared_ptr<Directory> beta1 = root->addDirectory("beta")->addDirectory("beta1");

    root->ls(4);

    return 0;
}
