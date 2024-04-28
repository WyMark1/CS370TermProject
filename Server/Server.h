#ifndef SERVER_H
#define SERVER_H

#include <string>

struct Task {
    std::string burstTimeStr;
    int arrival_time; 
    int burst_duration;
    std::string data;
    int progress;
};

int run();

#endif
