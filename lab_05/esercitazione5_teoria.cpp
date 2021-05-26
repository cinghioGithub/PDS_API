//
// Created by ff on 5/21/21.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

template <typename T>
class WaitForValue {
    mutex val_avail;
    mutex val_set;
    T val;

public:
    WaitForValue(){
        val_avail.lock();
    }

    T wait(){
        val_avail.lock();
        T copy = val;
        val_set.unlock();
        return  copy;
    };

    void signal(T val){
        val_set.lock();
        this->val = val;
        val_avail.unlock();
    };
};


void test_wait_for_value () {

    WaitForValue<int> wfv;

    thread consumer([&wfv]() -> void {

        while(true) {
            int v = wfv.wait();
            cout<<"T1 received: "<<v<<endl;
            if(v == -1 ) break;
            usleep(rand() % 1000000);
        }

    });

    thread producer([&wfv]() -> void {
        int i = 10;
        while(i-->=-1){
            wfv.signal(i);
            usleep(rand() % 1000000);
        }
    });

    consumer.join();
    producer.join();
}


void test_sync() {
    int count = 0;
    mutex m;

    auto fun = [&count, &m](){
        for(int i=0; i<100; i++){
            //m.lock();
            count+=1;
            //m.unlock();
        }
    };

    thread t1(fun);
    thread t2(fun);

    t1.join();
    t2.join();

    cout<<"count: "<<count<<endl;

}


int main() {

    test_sync();
    //test_wait_for_value();

}
