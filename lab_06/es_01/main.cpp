#include <iostream>
#include <future>
#include <functional>

template <class F>
class MyPackagedTask {
    F _f;
    using R = decltype(_f());
    std::promise<R> p;

public:
    MyPackagedTask & operator=(MyPackagedTask &&pt){
        if(this != pt){
            this->_f = std::move(pt._f);
            this->p = std::move(pt.p);
        }
        return this;
    }
    MyPackagedTask(F && fun): _f(fun){}
    std::future<R> get_future(){
        return p.get_future();
    }
    void operator()(){
        try{
            R ret = _f();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            p.set_value(std::move(ret));
        }
        catch(...) {
            p.set_exception(std::current_exception());
        }
    }
    void reset() { // permette di riusare il task
        //this->_f = nullptr;
        std::promise<R> tmp;
        this->p = std::move(tmp);
    }
};

int main() {
    auto func = [](){std::cout << "Function executed" << std::endl; return 5;};

    MyPackagedTask p(std::bind(func));
    std::future<int> f = p.get_future();
    p();
    int res = f.get();
    std::cout << res << std::endl;

    return 0;
}
