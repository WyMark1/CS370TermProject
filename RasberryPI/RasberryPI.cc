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

    bool hasSent = false;

    while (true) {

        if (receiveServerQueue.size() > 0 ) {
            string sent = receiveServerQueue.pop();
            cout << "Sending to client "<< sent << "\n";
            if (net.send(CLIENT_SEND_PORT, net.receive_ip, receiveServerQueue.pop()) == -1) return -1;
            hasSent = true;
        }
        sleep(1); // Wait for some things to come in
        int size = receiveQueue.size();
        if (size == 0 && hasSent) break;
        else if (size > 5) {
            string unsorted[5];
            for (int i = 0; i < 5; i++) {
                unsorted[i] = receiveQueue.pop();
            }
            //sort
            string sorted[5];
            for (int i = 0; i < 5; i++) {
                sendQueue.push(sorted[i]);
            }
        } else if (size > 0) {
            vector<string> unsorted;
            for (int i = 0; i < size; i++) {
                unsorted.push_back(receiveQueue.pop());
            }
            //sort
            vector<string> sorted = unsorted; // place holder
            for (const auto &item : unsorted) {
                sendQueue.push(item);
            }
        }
        
    } 

    while (receiveServerQueue.size() != 0){
        if (net.send(CLIENT_SEND_PORT, net.receive_ip, receiveServerQueue.pop()) == -1) return -1;
    }
    cout << "finished";
    doneSending = true;
    sendThread.join();
    receiveThread.join();
    receiveServer.join();
    /*
    // Calculate burst time as timestamp string
    time_t burstTimeRaw = time(nullptr);
    struct tm* burstTimeInfo = localtime(&burstTimeRaw);
    char timestampStr[80];
    strftime(timestampStr, 80, "%Y-%m-%d %H:%M:%S", burstTimeInfo);

    // Prepend the burst time to the data
    data = string(timestampStr) + " " + data;
    */ //add this to the new while loop

    return 0; 
} 
