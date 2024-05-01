#ifndef RASPBERRYPI_H
#define RASPBERRYPI_H

#define BUFFER_SIZE 1024

#include <string>

int calculate_burst_time(const std::string& data); 
int run();

#endif
