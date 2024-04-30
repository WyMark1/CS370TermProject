#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>  
#include <atomic>
#include <chrono>
#include <cstring>
#include "RasberryPI.h"
#include "../Networking/Networking.h"

using namespace std;

// Burst time calculation function
int calculate_burst_time(const string& data) {
    const double BYTES_PER_MS = 1000.0;
    int baseBurstTime = data.size() / BYTES_PER_MS;
    int networkVariability = rand() % 10 + 1;
    baseBurstTime += networkVariability;

    return baseBurstTime;
}

int run() {
    int CLIENT_PORT = 8081; 
    int SERVER_PORT = 8080;  
    int CLIENT_SEND_PORT = 8089;
    int SERVER_SEND_PORT = 8083;
    string SERVER_IP = "127.0.0.0";

    string data;
    Networking net;
    Networking net2;
    ThreadSafeQueue<string> sendQueue;
    ThreadSafeQueue<string> receiveQueue;
    ThreadSafeQueue<string> receiveServerQueue;
    atomic<bool> doneSending(false);
    thread receiveThread(receiver, ref(receiveQueue), ref(doneSending), ref(CLIENT_PORT), ref(net));
    thread sendThread(sender, ref(sendQueue), ref(doneSending), ref(SERVER_PORT), ref(SERVER_IP));
    thread receiveServer(receiver, ref(receiveServerQueue), ref(doneSending), ref(SERVER_SEND_PORT), ref(net2));

    while (true) {

        if (receiveServerQueue.size() > 0 ) {
            string sent = receiveServerQueue.pop();
            cout << "Sending to client "<< sent << "\n";
            this_thread::sleep_for(chrono::milliseconds(1));
            if (net.send(CLIENT_SEND_PORT, net.receive_ip, sent) == -1) return -1;
        }
        int size = receiveQueue.size();
        if (size > 0) {
            cout << "Size: " << size << "\n";
            vector<string> unsorted;
            for (int i = 0; i < size; i++) {
                unsorted.push_back(receiveQueue.pop());
            }

            // calculate burst time and add to line
            for (string& line : unsorted) {
                int burstTime = calculate_burst_time(line); 
                data = std::to_string(burstTime) + " " + line;
            }

            //sort
            vector<string> sorted = unsorted; // place holder
            for (const auto &item : unsorted) {
                this_thread::sleep_for(chrono::milliseconds(1));
                sendQueue.push(item);
            }
        }
        
    } 

    cout << "finished";
    doneSending = true;
    sendThread.join();
    receiveThread.join();
    receiveServer.join();

    return 0; 
} 
