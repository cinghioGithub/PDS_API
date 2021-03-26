#include <iostream>
#include "Message.h"


int main()
{
    Message m1(10);
    Message m2(20);

    Message m3(m2);
    Message *m4 = new Message(5);

    //stampa:
    std::cout << m1 << std::endl << m2 << std::endl << m3 <<std::endl;

    //delete m4;

    return 0;
}
