#include <iostream>
#include <mutex>

class LockGuard{
    std::mutex *m;
public:
    LockGuard(std::mutex &m){
        this->m = &m;
        this->m->lock();
        std::cout << "lock()" << std::endl;
    }
    ~LockGuard(){
        this->m->unlock();
        std::cout << "unlock()" << std::endl;
    }
};

int main() {
    std::mutex m;
    LockGuard lg{m};
    return 0;
}
