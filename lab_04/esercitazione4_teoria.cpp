//
// Created by ff on 5/10/21.
//
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <memory>
#include <functional>

using namespace std;

// https://man7.org/linux/man-pages/dir_section_2.html

int simple_fork() {

    pid_t child_pid = fork();
    int wstatus;

    if(child_pid == -1) {
        cerr<<"for error";
        exit(EXIT_FAILURE);
    } else if(child_pid > 0) {
        cout<<"[F] I'm the father "<<getpid()<<endl;
        wait(&wstatus);
        cout<<"[F] child status: "<<wstatus<<endl;

    } else {
        cout<<"[K] I'm the kid! " << getpid() << endl;
        //exit(EXIT_FAILURE);
        sleep(5);
        cout<<"[K] exiting"<<endl;
        exit(EXIT_SUCCESS);
    }
}

template <typename F, typename C>
void my_fork(F father_fun, C child_fun) {

    pid_t father_pid = getpid();
    pid_t child_pid = fork();
    if(child_pid == -1) {
        cerr<<"for error";
        exit(EXIT_FAILURE);
    } else if(child_pid > 0) {
        wait(nullptr);
        // executed after wait!
        father_fun(child_pid);
    } else {
        child_fun(father_pid);
    }
}

void test_mem(){
    shared_ptr<string> my_string= make_shared<string>("shared string: initial");

    my_fork(
            [my_string](int pid) -> void {
                cout<<"[F] I'm the father!!!"<<endl;
                cout<<"[F] string: "<<*my_string<<endl;
            },

            [my_string](int pid) -> void {
                cout<<"I'm the kid!!!"<<endl;
                cout<<"[K] string: "<<*my_string<<endl;
                sleep(5);
                (*my_string) += " some other text";
                cout<<"[K] string: "<<*my_string<<endl;
            }
    );

}

//template <typename T>
//void runner(function<T(int)> const& fun) {
//    cout<<"r: "<<fun(10);
//}

int fail(const string &msg){
    cerr<<msg<<endl;
    exit(EXIT_FAILURE);
}


string myreadline(int fp, int max) {
    char buf[max+1];
    int i = 0;
    do {
        read(fp, buf + i, 1);
    } while (++i<max && buf[i-1] != '\n');
    buf[i] = 0;
    return buf;
}

int pipe_echo() {

    string message;
    char buf[1000];

    // crea pipe
    int c2p[2]; // [1]: write, [0]: read
    int p2c[2];
    pipe(c2p)==0  || fail("can't open pipe");
    pipe(p2c)==0 || fail("can't open pipe");

    cout<<"prompt string: ";
    cin>>message;
    if(message.length()>100) fail("message too long");
    message += "\n";

    int cpid = fork();

    switch(cpid) {
        case -1:
            fail("can't fork");
        case 0: // father
            close(p2c[0]);
            close(c2p[1]);

            write(p2c[1], message.c_str(), message.length());
            cout<<"echo received: "<<myreadline(c2p[0], 100);
            wait(nullptr);
            break;
        default: // child

            close(p2c[1]);
            close(c2p[0]);

            //int n = read(p2c[0], buf, 1000 );
            string resp = myreadline(p2c[0], 100);
            write(c2p[1], resp.c_str(), resp.length());
    }

}


int main() {
    //simple_fork();
    //test_mem();

    pipe_echo();
    //((runner<float>([](int v) -> float {cout<<"aaa "<<v<<endl; return 42.1; });

}