#include <iostream>
#include <thread>
#include <queue>
#include <future>

std::queue<std::packaged_task<void(int)>> queue_job;
std::mutex queue_mu;
std::condition_variable queue_cv;
std::mutex cout_mu;
std::atomic<bool> end = false;

void thread_handler(int i){
    std::packaged_task<void(int)> task ;
    bool first = true;

    while(true){
        if(!first){
            task(i);
        }
        std::unique_lock<std::mutex> ul(queue_mu);
        queue_cv.wait(ul, [](){return !queue_job.empty() || end;});
        if(end && queue_job.empty()) break;
        task = std::move(queue_job.front());
        queue_job.pop();
        if(first) first = false;
    }
}

int main() {
    //may return 0 when not able to detect
    const auto processor_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for(int i=0; i<processor_count; i++){
        std::thread t(thread_handler, i);
        threads.push_back(std::move(t));
    }

    for(int i=0; i<2*processor_count; i++){
        std::lock_guard<std::mutex> lg(queue_mu);
        queue_job.push(std::packaged_task<void(int)> ([](int a) {
            std::lock_guard<std::mutex> lg(cout_mu);  //lock per effettuare stampe atomiche
            std::cout << "Task eseguito dal thread: " << a << std::endl;
        }));
        queue_cv.notify_all();
    }

    end = true;

    for(auto & thread : threads){
        thread.join();
    }
    return 0;
}
