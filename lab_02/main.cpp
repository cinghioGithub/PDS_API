#include <iostream>
#include "Directory.h"

int main() {

    Directory* root = Directory::getRoot();
    Directory* alfa = root->addDirectory("alfa");
    root->addDirectory("beta")->addDirectory("beta1")->addDirectory("beta2");
    Directory* gamma = root->addDirectory("gamma");
    /*root
          alfa
          beta
              beta1
                   beta2
          gamma
    */
    root->copy("beta", gamma);
    /*root
           alfa
           beta
               beta1
                    beta2
           gamma
                beta
                  beta1
                       beta2
    */
    //root->getDir("beta")->addDirectory("beta2");
    //alfa->getDir("..")->ls(4);
    std::cout << (alfa->remove("beta") ? "true" : "false") << std::endl;
    root->ls(4);

    delete root;

    return 0;
}
