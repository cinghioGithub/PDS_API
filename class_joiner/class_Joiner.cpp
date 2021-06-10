#include <iostream>
#include <map>
#include <thread>
#include <condition_variable>
#include <mutex>
class Joiner {
    std::condition_variable cv;
    std::mutex m;
    int count;
    int N;
    std::map<std::thread::id,double>  values;
    bool entering;
public:
    Joiner(int N): N(N), count(0), entering(true) {}// N is the number of values that must be conferred
    std::map<std::thread::id, double> supply(double value) {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this](){return entering;});
        values[std::this_thread::get_id()] = value;
        count++;
        if (count<N) {
            cv.wait(ul, [this](){return !entering;});
        } else {
            cv.notify_all();
            entering=false;
        }
        count--;
        if (count==0) {
            entering = true;
            cv.notify_all();
            std::map<std::thread::id,double> values1(values);
            values.clear();
            return values1;
        }
        return values;
    }
};
int main() {
    const int N = 4;
    Joiner joiner(N);
    int iter = 10000;
    auto f = [&joiner,N,iter](bool print) {
        for (int i=0; i<iter; i++) {
            auto m = joiner.supply(i);
            assert(m.size() == N);
            for (auto p: m) {
                if (p.second!=i) std::cout<<"Expected "<<i<<" found "<<p.second<<std::endl;
                assert(p.second==i);
            }
            if(print) {
                std::cout<<i<<" ";
                if (i%10==9) std::cout<<std::endl;
            }
        }
    };
    std::thread threads[N];
    for (int i=0; i<N; i++)
        threads[i]=std::thread(f, i==0);
    for (auto & thread : threads)
        thread.join();
    return 0;
}