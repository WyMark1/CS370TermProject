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
        this_thread::sleep_for(chrono::milliseconds(3));
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

            sort(unsorted.begin(), unsorted.end(), 
                 [](const string& a, const string& b) {
                     size_t delimPosA = a.find("BurstTImePI");
                     size_t delimPosB = b.find("BurstTImePI");
                     string burstTimeStrA = a.substr(0, delimPosA); 
                     string burstTimeStrB = b.substr(0, delimPosB); 
                     int burstTimeA = stoi(burstTimeStrA);
                     int burstTimeB = stoi(burstTimeStrB);
                     return burstTimeA < burstTimeB;
            });

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
