#include <iostream>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <thread>
#include <atomic>

class CountingSemaphore {
    int count;
    std::mutex m;
    std::condition_variable cv;
public:
    CountingSemaphore(){
        this->count = 0;
    }
    CountingSemaphore(int n){
        this->count = n;
    }
    void acquire(){
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this](){return this->count>0;});
        this->count--;
    }
    void release(){
        std::unique_lock<std::mutex> ul(m);
        this->count++;
        cv.notify_all();
    }
};

CountingSemaphore cs(4);
std::atomic<int> res = 0;
std::atomic<int> count_thread = 0;
//std::mutex m_res;

void foundString(int index, std::string value){
    int count = 0;
    std::ifstream fin;
    fin.open("../file.txt", std::ifstream::in);
    while(fin.good()) {
        if(res == 1){
            std::cout << "Stringa trovata thread: " << index << std::endl;
            break;
        }
        else{
            cs.acquire();
            count_thread++;
            std::cout << "thread: " << index  << " count_thread: " << count_thread << std::endl;
            std::cout << "Stringa NON trovata thread: " << index << std::endl;
            char line[100];
            fin.getline(line, 200);
            count++;
            std::string str(line);
            if(count % index == 0){
                if(str.find(value) != std::string::npos){
                    res = 1;
                }
            }
        }
        cs.release();
        count_thread--;
    }
}

int main() {
    std::string value = "riceve";

    std::thread t1(foundString, 1, value);
    std::thread t2(foundString, 2, value);
    std::thread t3(foundString, 3, value);
    std::thread t4(foundString, 4, value);
    std::thread t5(foundString, 5, value);
    std::thread t6(foundString, 6, value);
    std::thread t7(foundString, 7, value);
    std::thread t8(foundString, 8, value);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();

    if(res == 1){
        std::cout << "Stringa trovata" << std::endl;
    }
    else{
        std::cout << "Stringa NON trovata" << std::endl;
    }

    return 0;
}
