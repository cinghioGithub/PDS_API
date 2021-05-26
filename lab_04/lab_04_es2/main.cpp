#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
#include <filesystem>
#include <fstream>
#include <queue>
#include <set>
#include <math.h>

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
    string toString() const {
        return to_string(start) + " " + to_string(end);
    }
    int getStart(){
        return start;
    }
    int getEnd(){
        return end;
    }
    friend ostream& operator<<(ostream& os, const Task& td) {
        return cout << td.priority << td.start << td.end;
    }
};

vector<int> getPrime(int start, int end){
    vector<int> prime{};
    for(int num=start; num<=end; num++){
        int c = 0;
        int max = (int)sqrt(num);
        for(int j=1; j<=max; j++){
            if(num%j == 0){
                c++;
            }
        }
        if(c == 1){
            prime.push_back(num);
        }
    }
    return prime;
}

int main() {
    int numFigli;
    int index;
    pid_t res;
    cout << "Numero figli: ";
    cin >> numFigli;

    vector<pid_t> children{numFigli};
    int p2c[numFigli][2];
    int c2p[2];
    //filesystem::directory_iterator dir {"../tasks"};
    pid_t father = getpid();
    priority_queue<Task> queue{};
    set<filesystem::path> files{};
    bool foundStop = false;

    for(int i=0; i<numFigli; i++){
        if(pipe(c2p) < 0){
            cout << "Errore pipe c2p" << endl;
            exit(-1);
        }
        if(pipe(p2c[i]) < 0){
            cout << "Errore pipe p2c[" << i << "]" << endl;
            exit(-1);
        }

        res = fork();
        if(res == -1) {
            cout << "Errore fork" << endl;
            exit(-1);
        } else if (res == 0){   //child
            close(c2p[0]);
            close(p2c[i][1]);
            index = i;
            //cout << i << endl;
            break;
        } else{         //parent
            close(p2c[i][0]);
            close(c2p[1]);
            //waitpid(res, nullptr, 0);
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
                        else queue.push(Task{priority, start, end});
                    }
                }
            }
            while(!queue.empty()) {
                //cout << queue.top() << endl;
                int free_child;
                int n = read(c2p[0], (void *) &free_child, sizeof(int));
                cout << "padre letto: " << n << endl;
                cout << "freeChild: " << free_child << endl;
                auto tmp = queue.top();
                int start = tmp.getStart();
                int end = tmp.getEnd();
                //cout << "task: " << queue.top().toString().c_str() << endl;
                write(p2c[free_child][1], (void *) &start, sizeof(int));
                write(p2c[free_child][1], (void *) &end, sizeof(int));
                queue.pop();
                int val = 0;
                while(val != -1){
                    int n = read(c2p[0], (void *) &val, sizeof(int));
                    cout << "numero primo: " << val << endl;
                }
            }
            //exit(0);
        } else{
            int self = index;
            int n = write(c2p[1], (void *) &self, sizeof(int));
            cout << "figlio " << self << " scritto: " << n << endl;
            while(1){
                cout << "id filgio: " << index << endl;
                //int self = index;
                //int n = write(c2p[1], (void *) &self, sizeof(int));
                //cout << "figlio " << self << " scritto: " << n << endl;
                int start;
                read(p2c[index][0], (void *) &start, sizeof(int) );
                int end;
                read(p2c[index][0], (void *) &end, sizeof(int) );
                cout << "figlio[" << index << "]: " << start << " " << end << endl;

                vector<int> prime = getPrime(start, end);

                for(int v : prime){
                    write(c2p[1], (void *) &v, sizeof(int));
                }
                int fine = -1;
                int n = write(c2p[1], (void *) &fine, sizeof(int));
                write(c2p[1], (void *) &self, sizeof(int));
            }
        }
    }

    int free_child;
    int n = read(c2p[0], (void *) &free_child, sizeof(int));

    return 0;
}
