#ifndef NETWORKING_H
#define NETWORKING_H
#include <string>


class Networking {
    public:
        std::string receive_ip;
        int send(int port, std::string ip, std::string data);
        std::string receive(int port);
};

#endif