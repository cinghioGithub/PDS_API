#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <memory>
#include <functional>

using namespace std;

string my_read_line(int fd, int max){
    char buff[max+1];
    int i=0;
    do{
        read(fd, buff+i, 1);
    }while(++i<max && buff[i-1]!='\n');
    buff[i]='\0';
    return buff;
}

template <typename T>
T runInChild(std::function<T(void)> fun){
    pid_t pid_father = getpid();
    string res_fun;
    int c2f[2];

    if(pipe(c2f) != 0){
        cout << "Errore pipe" << endl;
        exit(-1);
    }

    pid_t pid = fork();

    switch (pid){
        case -1:
            cout << "Errore fork!" << endl;
            exit(-1);
        case 0:
            close(c2f[0]);  //chiudo in lettura
            res_fun = fun();
            //cout << res_fun << endl;
            write(c2f[1], res_fun.c_str(), res_fun.length());
            //exit(0);
            break;
        default:
            close(c2f[1]);   //chiudo in scrittura
            wait(nullptr);
            cout << "Figlio ha finito" << endl;
            return my_read_line(c2f[0], 100);
    }
}

int main() {

    function<string()> fun{ []() {return "Child\n";}};
    string res = runInChild(fun);
    cout << "Figlio: " << res << endl;

    return 0;
}
