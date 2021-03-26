#include <iostream>

class Buffer{
private:
    int size;
    int *ptr;
public:
    //costruttore
    Buffer(int size): size(size) {
        ptr = new int[size];
        std::cout << "Creato oggetto con vettore di dimensione " << this->size << std::endl;
    }

    //distruttore
    ~Buffer(){
        delete[] ptr;
        std::cout << "Distrutto oggetto con vettore di dimensione " << this->size << std::endl;
    }

    //costruttore di copia
    Buffer(const Buffer& source): size(source.size){
        this->ptr = new int[source.size];
        for(int i=0; i<source.size; i++)
            this->ptr[i] = source.ptr[i];
    }

    //costruttore di movimento
    Buffer(Buffer&& source): size(source.size){
        this->ptr = source.ptr;
        source.ptr = NULL;
        std::cout << "Spostato l'oggetto " << std::endl;
    }

    int getSize(){
        return this->size;
    }

    bool getData(int pos, int& val){
        if(pos > this->size - 1)
            return false;
        val = this->ptr[pos];
        return true;
    }

    bool setData(int pos, int val){
        if(pos > this->size - 1)
            return false;
        this->ptr[pos] = val;
        return true;
    }
};

int main() {

    Buffer b1(10);
    Buffer b2{12};
    Buffer *b3 = new Buffer(13);

    b3->setData(5, 12);

    Buffer b4{ *b3 };

    int val1, val2;
    b3->getData(5, val1);
    b4.getData(5, val2);

    Buffer b5 = *new Buffer(5);

    std::cout << "b3->ptr[5] = " << val1 << std::endl;
    std::cout << "b4.ptr[5] = " << val2 << std::endl;

    delete b3;
    b3 = NULL;

    return 0;
}
