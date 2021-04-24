#include <iostream>
#include "Directory.h"

int main() {

    Directory* root = Directory::getRoot();
    Directory* alfa = root->addDirectory("alfa");
    Directory* beta1 = root->addDirectory("beta")->addDirectory("beta1");
    beta1->addDirectory("beta2")->addFile("file1", 8);
    beta1->addFile("file1", 8);
    Directory* gamma = root->addDirectory("gamma");
    gamma->addFile("file1", 8);
    root->ls(4);
    //root->copy("beta", gamma);
    //root->getDir("beta")->addDirectory("beta2");
    //alfa->getDir("..")->ls(4);
    std::cout << (alfa->remove("beta") ? "true" : "false") << std::endl;
    //root->ls(4);

    return 0;
}
