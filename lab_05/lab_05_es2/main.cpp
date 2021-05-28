#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <condition_variable>

int count=0;
std::vector<int> vect{};
bool ready = false;
std::mutex m;
std::condition_variable cv;
//int f1Tof2[2];
//int f2Tof1[2];

void f1(){
    char buff[1];
    for(int i=0; i<1000000; i++){
        //read(f2Tof1[0], buff, 1);
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [](){return !ready;});
        count++;
        //std::cerr << "prodotto " << count << " i = " << i << std::endl;
        ready = true;
        cv.notify_all();
        //write(f1Tof2[1], "a", 1);
    }
}

void f2(){
    int somma=0;
    char buff[1];
    for(int i=0; i<1000000; i++){
        //read(f1Tof2[0], buff, 1);
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [](){return ready;});
        somma += count % 43;
        //std::cerr << "consumato " << count << " i = " << i << std::endl;
        ready = false;
        cv.notify_all();
        //write(f2Tof1[1], "a", 1);
    }
    std::cout << "somma resti = " << somma << std::endl;
}

void f3(){
    int c = 0, somma = 0;
    for(int i=0; i<1000000; i++){
        c++;
        somma += c % 43;
    }
    std::cout << "somma resti = " << somma << std::endl;
}

void f4(){
    int c = 0, somma = 0;
    for(int i=0; i<1000000; i++){
        c++;
        vect.push_back(c);
        somma += vect[i] % 43;
    }
    std::cout << "somma resti = " << somma << std::endl;
}

int main() {
    //pipe(f1Tof2);
    //pipe(f2Tof1);

    f3(); //check the result
    //write(f2Tof1[1], "a", 1);
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(f1);
    std::thread t2(f2);

    t1.join();
    t2.join();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << ((float)duration.count())/((float)1000000) << " sec" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    f4();
    stop = std::chrono::high_resolution_clock::now();
    duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << ((float)duration.count())/((float)1000000) << " sec" << std::endl;

    return 0;
}
