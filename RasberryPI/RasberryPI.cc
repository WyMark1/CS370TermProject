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
#define SERVER_IP "127.0.0.0" //IP of server

using namespace std;

int run() {
    string data;
    Networking net;
    data = net.receive(CLIENT_PORT);
    cout << "Data from client: " << data << "\n";
    string client_ip = net.receive_ip;
    //Edit data
    if(net.send(SERVER_PORT,SERVER_IP, data) == -1) return -1;

    data = net.receive(SERVER_PORT);
    cout << "Data from server: " << data << "\n";
    if(net.send(CLIENT_PORT, client_ip, data) == -1) return -1;
    return 0;
}

