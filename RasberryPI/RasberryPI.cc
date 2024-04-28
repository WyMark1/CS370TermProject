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
#define SERVER_IP "127.0.0.0" //IP of server


using namespace std;


int run() {
    string data;
    Networking net;

    data = net.receive(CLIENT_PORT);
    string client_ip = net.receive_ip;

    // Calculate burst time as timestamp string
    time_t burstTimeRaw = time(nullptr);
    struct tm* burstTimeInfo = localtime(&burstTimeRaw);
    char timestampStr[80];
    strftime(timestampStr, 80, "%Y-%m-%d %H:%M:%S", burstTimeInfo);

    // Prepend the burst time to the data
    data = string(timestampStr) + " " + data;

    // Forward to server
    if(net.send(SERVER_PORT,SERVER_IP, data) == -1) return -1;

    // Receive from server 
    data = net.receive(SERVER_SEND_PORT);

    // Send to client
    if(net.send(CLIENT_SEND_PORT, client_ip, data) == -1) return -1;
    
    return 0; 
} 
