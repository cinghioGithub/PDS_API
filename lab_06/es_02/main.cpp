#include <iostream>
#include <vector>
#include <math.h>
#include <future>

void sort(std::vector<int>::iterator start,  std::vector<int>::iterator end, int n, int p){
    if(std::pow(2, p) == n) {
        std::sort(start, end);
        return;
    }

    std::vector<int>::iterator middle = (end - start) / 2 + start ;
    std::future f = std::async(std::launch::async, sort, start, middle, n, p+1);
    sort(middle, end, n, p+1);
    f.get();
    std::sort(start, end);
}

int main() {
    std::vector<int> vet;
    for(int i = 0; i<100000; i++){
        vet.push_back(100000-i);
    }

    auto i = std::chrono::system_clock::now();
    sort(vet.begin(), vet.end(), 128, 0);
    auto f = std::chrono::system_clock::now();
    std::cout << "durata: " << (f-i).count() << std::endl;
    //for(auto val : vet){
      //  std::cout << val << " ";
    //}
    std::cout << std::endl;

    //vet = {12, 3, 2, 34, 54, 4, 5, 9, 0, 12, 22, 43, 55, 65, 123, 90, 12, 3, 2, 34, 54, 4, 5, 9, 0, 12, 22, 43, 55, 65, 123, 90};
    for(int i = 0; i<100000; i++){
        vet.push_back(100000-i);
    }
    i = std::chrono::system_clock::now();
    sort(vet.begin(), vet.end(), 4, 0);
    f = std::chrono::system_clock::now();
    std::cout << "durata: " << (f-i).count() << std::endl;
    return 0;
}
