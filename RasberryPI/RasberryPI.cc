#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstring> 
#include "RasberryPI.h"
#include "../Networking/Networking.h"

#define CLIENT_PORT 8081  // Port for Client
#define SERVER_PORT 8080  // Port for Server
#define CLIENT_SEND_PORT 8089
#define SERVER_SEND_PORT 8083
#define SERVER_IP "10.0.0.176" //IP of server

using namespace std;

int run() {
    string data;
    Networking net;
    data = net.receive(CLIENT_PORT);
    string client_ip = net.receive_ip;
    //Edit data
    if(net.send(SERVER_PORT,SERVER_IP, data) == -1) return -1;
    data = net.receive(SERVER_SEND_PORT);
    if(net.send(CLIENT_SEND_PORT, client_ip, data) == -1) return -1;
    return 0;
}

