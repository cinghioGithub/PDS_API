#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <filesystem>
#include <fstream>
#include <queue>
#include <set>

using namespace std;

class Task {
private:
    int priority;
    int start;
    int end;
public:
    Task(int p, int s, int e): priority(p), start(s), end(e){}
    friend bool operator<(const Task& x, const Task& y) {
        return x.priority > y.priority;
    }
    friend ostream& operator<<(ostream& os, const Task& td) {
        return cout << td.priority << td.start << td.end;
    }
};

int main() {
    int numFigli;
    pid_t res;
    cout << "Numero figli: ";
    cin >> numFigli;

    vector<pid_t> children{numFigli};
    int p2c[numFigli][2];
    int c2p[2];
    close(c2p[1]);
    filesystem::directory_iterator dir {"../tasks"};
    pid_t father = getpid();
    priority_queue<Task> queue{};
    set<filesystem::path> files{};
    bool foundStop = false;

    /*p2c = (int **)malloc(sizeof(int *)*numFigli );
    for(int i=0; i<numFigli; i++){
        p2c[i] = (int *) malloc(sizeof(int)*2);
    }*/

    if(pipe(c2p) < 0){
        cout << "Errore pipe c2p" << endl;
        exit(-1);
    }

    for(int i=0; i<numFigli; i++){
        if(pipe(p2c[i]) < 0){
            cout << "Errore pipe p2c[" << i << "]" << endl;
            exit(-1);
        }
        res = fork();
        if(res == -1) {
            cout << "Errore fork" << endl;
            exit(-1);
        } else if (res == 0){
            close(c2p[0]);
            close(p2c[i][1]);
            cout << i << endl;
            break;
        } else{
            close(p2c[i][0]);
            close(c2p[1]);
            waitpid(res, nullptr, 0);
            children.push_back(res);
        }
    }

    while(!foundStop){
        if(getpid() == father){
            for(auto& file : filesystem::directory_iterator{"../tasks"}){
                //cout << file.path() << endl;
                if(files.insert(file.path()).second) {
                    ifstream fin;
                    fin.open(file.path(), fstream::in);
                    while (fin.good()) {
                        int priority;
                        int start;
                        int end;
                        fin >> priority;
                        fin >> start;
                        fin >> end;
                        if(priority == 0 && start == 0 && end == 0) foundStop = true;
                        queue.push(Task{priority, start, end});
                    }
                }
            }
            while(!queue.empty()) {
                cout << queue.top() << endl;
                queue.pop();
            }
            //exit(0);
        } else{
            exit(0);
        }
    }

    return 0;
}
