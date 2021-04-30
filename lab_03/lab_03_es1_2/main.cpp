#include <iostream>
#include "My_shared.h"
#include "Tracker.h"

class Base: public Tracker<Base>{
    int val;
public:
    Base(int v): val(v){
        std::cout << "costruttore oggetto -> " << count << std::endl;
    }
    ~Base(){
        std::cout << "distruttore oggetto -> " << count << std::endl;
    }
    int getVal(){
        return this->val;
    }
};

int main() {
    My_shared<Base> t{new Base(5)};
    My_shared<Base> t1{t};
    std::cout << t1->getVal() << std::endl;
    std::cout << t1.use_count() << std::endl; //2
    t1 = std::move(t);
    std::cout << t1.use_count() << std::endl;  //1
    std::cout << t1->getVal() << std::endl;
    return 0;
}
