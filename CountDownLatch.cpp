#include <iostream>
#include <mutex>

class CountDownLatch{
    int count;
    std::mutex m;
    std::contidion_variable cv;

public:
    CountDownLatch(int initialCount): count(initialCount){}
    void await(){
        /*utiliziamo un unique_lock perchè
        ho bisogno di chamare la wait della cv,
        che ha bisogno di un oggetto unique lock*/
        std::unique_lock<std::mutex> ul(m);
        /*while(count > 0){ //questa è una possibilità
            cv.wait(ul);
        }*/

        /*catturo this, in modo da aver a disposizione count (data che non è una variabile globale)*/
        cv.wait(ul, [this](){return count == 0;});
    }

    void countDown(){
        std::lock_guard<std::mutex> lg(m);
        if(count > 0){
            count--;
            if(count == 0) cv.notify_all();
        }
    }
    int getCount(){
        std::lock_guard<std::mutex> lg(m);
        return count;
    }
}

int main(){
    CountDownLatch cl(3);
    for(int i=0; i<5; i++){
        std::thread t([i, &cl]() {
            cl.await();
            std::count << "Thread " << i << " finished" << std::endl;
        });
        t.detach();
    }

    while(true){
        std::string s;
        std::getline(std::cin, s);
        cl.countDown();
        std::cout << "Current count is " << cl.getCount() << std::endl;
        /*il flush dovrebbe già farlo std::endl*/
        std::cout.flush();
    }
}