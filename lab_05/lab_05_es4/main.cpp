#include <iostream>
#include <thread>
#include <filesystem>
#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>

auto start = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
std::queue<std::string> queue;
std::mutex mu;
std::condition_variable cv;

void manage_source(std::filesystem::path file){
    std::ifstream fin;
    fin.open(file);
    while(fin.good()){
        int timestamp;
        std::string source_path;
        std::string dest_path;

        fin >> timestamp >> source_path >> dest_path;
        auto now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int timestamp_now = now - start;

        if(timestamp_now < timestamp)
            std::this_thread::sleep_for(std::chrono::milliseconds (timestamp - timestamp_now) );

        now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        timestamp_now = now - start;
        std::lock_guard<std::mutex> ul(mu);
        queue.push("Now: " + std::to_string(timestamp_now) + " " + "Timestamp: " + std::to_string(timestamp) + " " + source_path + " " + dest_path);
        cv.notify_one();
    }
}

void consume(){
    std::string read;
    while(read != "end") {
        std::unique_lock ul(mu);
        cv.wait(ul, []() { return !queue.empty(); });
        read = queue.front();
        std::cout << read << std::endl;
        queue.pop();
    }
}

int main() {
    std::vector<std::thread> threads;

    for(auto file : std::filesystem::directory_iterator("./sources")){
        std::cout << file << std::endl;
        std::thread t(manage_source, file.path());
        threads.push_back(std::move(t));
    }

    std::thread t(consume);

    for(int i=0; i<threads.size(); i++){
        threads[i].join();
    }

    mu.lock();
    queue.push("end");
    cv.notify_one();
    mu.unlock();

    t.join();

    return 0;
}
