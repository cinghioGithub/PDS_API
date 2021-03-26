#include <iostream>
#include "Message.h"
#include "DurationLogger.h"


int main()
{
    DurationLogger d("main()");

    //costruttre di dafault
    Message m7;

    //costruttore
    Message m1(10);
    Message m2(20);
    Message m3(10);

    //costruttore di copia
    Message m4(m3);

    //costruttore di movimento
    Message m5 = std::move(m3);   //m3 viene svuotato
    Message m6(std::move(m5));    //m5 viene svuotato

    //operatore di asseganzione
    m1 = m2;

    //operatore di assegnazione per movimento
    m1 = std::move(m7);     //m7 viene svuotato

    //allocazione dinamica
    Message *m8 = new Message(10);

    //stampa:
    std::cout << m1 << std::endl << m2 <<std::endl;

    Message buff1[10];
    Message *buff2 = new Message[10];

    Message m9(10);
    buff2[0] = m9;

    buff1[0] = Message(100);

    clock_t t1 = clock();
    //time_t t1 = time(NULL);
    for(int i=0; i<10; i++){
        buff1[i] = Message(1024*1024);
    }
    clock_t t2 = clock();
    //time_t t2 = time(NULL);
    std::cout << "Tempo assegnazione per movimento: " << (float)(t2-t1)/CLOCKS_PER_SEC << std::endl;
    //std::cout << "Tempo assegnazione per movimento: " << (t2-t1) << std::endl;
    t1 = clock();
    //t1 = time(NULL);
    for(int i=0; i<10; i++){
        buff2[i] = buff1[i];
    }
    t2 = clock();
    //t2 = time(NULL);
    std::cout << "Tempo assegnazione: " << (float)(t2-t1)/CLOCKS_PER_SEC << std::endl;
    //std::cout << "Tempo assegnazione: " << (t2-t1) << std::endl;

    delete m8;
    //std::cout<<"debug"<<std::endl;
    delete[] buff2;

    return 0;
}
