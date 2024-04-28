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

// Simple struct to hold task data
struct Task {
    int burst_time;
    string data;
    string client_ip; 
};

// Comparison for the priority queue (Orders by shortest burst time)
struct CompareBurstTime {
    bool operator()(const Task& t1, const Task& t2) {
        return t1.burst_time > t2.burst_time; 
    }
};

int estimate_burst_time(const string& data) {
    return data.size() / 10;  // Process speed
}

int run() {
    priority_queue<Task, vector<Task>, CompareBurstTime> task_queue;
    Networking net;

    while (true) { // Main processing loop
        if (task_queue.empty()) {
            // Wait for a new task 
            string data = net.receive(CLIENT_PORT);
            string client_ip = net.receive_ip;
            int burst_time = extract_burst_time(data); // You'll need to implement this
            task_queue.push({burst_time, data, client_ip});           
        } else {
            // Process the next task based on SJF
            Task current_task = task_queue.top();
            task_queue.pop();

            // Placeholder edit (replace later with your  processing)
            current_task.data[0] = '?'; 

            // Send to server, get response, send back to the client
            if (net.send(SERVER_PORT, SERVER_IP, current_task.data) == -1) return -1;
            string server_response = net.receive(SERVER_SEND_PORT);
            if (net.send(CLIENT_SEND_PORT, current_task.client_ip, server_response) == -1) return -1;
        }
    } 

    return 0;
}

