//
// Created by Enrico on 26/03/2021.
//

#ifndef LAB_01_DURATIONLOGGER_H
#define LAB_01_DURATIONLOGGER_H

#include <iostream>


class DurationLogger {
    clock_t inizio;
    std::string name;
public:
    DurationLogger(const std::string& name);
    ~DurationLogger();
};


#endif //LAB_01_DURATIONLOGGER_H
