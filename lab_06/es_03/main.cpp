#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <future>
#include <functional>

class thread_pool{
    int min, max, active_threads, launched_threads;
    std::vector<std::thread> threads;
    std::queue<std::packaged_task<void()>> task_queue;
    std::mutex mu;
    std::condition_variable cv;
    std::atomic<bool> end = false;
    std::function<void()> func = [this](){
        std::packaged_task<void()> task;
        bool first = true;

        while(true){
            if(!first) {
                task();
                std::lock_guard<std::mutex> lg(mu);
                active_threads--;
            }
            std::unique_lock<std::mutex> ul(mu);
            cv.wait(ul, [this](){return !task_queue.empty() || end;});
            if(end && task_queue.empty()) break;
            active_threads++;
            task = std::move(task_queue.front());
            if(first) first = false;
            task_queue.pop();
        }
    };
public:
    thread_pool(int min, int max){
        this->min = min;
        this->max = max;
        this->active_threads = 0;
        this->launched_threads = min;

        for(int i=0; i<min; i++){
            std::thread t(func);
            threads.push_back(std::move(t));
        }
    }

    void execute(std::packaged_task<void()> && task){
        if(!end) {
            std::lock_guard<std::mutex> lg(mu);
            if (active_threads == launched_threads && launched_threads != max) {
                std::thread t(func);
                threads.push_back(std::move(t));
                launched_threads++;
            }
            task_queue.push(std::move(task));
            cv.notify_all();
        }
        else{
            throw std::exception();
        }
    }

    void finish(){
        end = true;
        cv.notify_all();
        for(int i=0; i<threads.size(); i++){
            threads[i].join();
        }
    }
};

int main() {
    thread_pool tp(2,4);
    tp.execute(std::packaged_task<void()>(std::bind([](){std::cout << "task1" << std::endl;})));
    tp.execute(std::packaged_task<void()>(std::bind([](){std::cout << "task2" << std::endl;})));
    tp.execute(std::packaged_task<void()>(std::bind([](){std::cout << "task3" << std::endl;})));
    tp.execute(std::packaged_task<void()>(std::bind([](){std::cout << "task4" << std::endl;})));
    tp.finish();
    return 0;
}
