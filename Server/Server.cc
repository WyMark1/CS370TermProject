#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <stack>
#include <chrono>
#include "Server.h"
#include "../Networking/Networking.h"
#include "../Encryption/Encryption.h"

using namespace std;

int parseBurstDuration(const string& data) {
    int pos = data.find(' ');
    if (pos == -1) {
        cerr << "Error: Invalid data format. Expected burst time before data.\n";
        return -1;
    }

    string burstStr = data.substr(0, pos);
    return stoi(burstStr);
 
}

int run() {
    Networking net;
    string key = "SUPER secret key";
    int SERVER_PORT = 8080;
    int SERVER_SEND_PORT = 8083;
    ThreadSafeQueue<string> receiveQueue;
    atomic<bool> doneSending(false);
    thread receiveThread(receiver, ref(receiveQueue), ref(doneSending), ref(SERVER_PORT), ref(net));
    string current;
    stack<string> working;
    while (true) {
        int size = receiveQueue.size();
        if (size > 0 && current.empty()) {
            string decrypt;
            string encrypted = receiveQueue.pop();
            Decrypt(encrypted, decrypt, key);
            current = decrypt;
            cout << "Current: "<< current << "\n";
            //do work on current
        } else if (size > 0 && !current.empty()) {
            // Parse burst durations
            string nextItem = receiveQueue.pop();
            int nextBurst = parseBurstDuration(nextItem);
            int currentBurst = parseBurstDuration(current); 
            current = current.substr(current.find(' ') + 1); // Remove burst time

            // Task Scheduling 
            if (nextBurst < currentBurst) {
                working.push(current);
                current = receiveQueue.pop();
                cout << "Moved current to stack. New current: " << current << endl;

                receiveQueue.push(nextItem);
            }
            
        } else if (!current.empty()) { // And the burst duration is done send to the PI
            // Task Processing
            cout << "Processing task: " << current << endl;
            sleep(1); // Simulate processing time

            string send;
            cout << "sent " << current << "\n";
            Encrypt(current, send, key);
            if(net.send(SERVER_SEND_PORT, net.receive_ip, send) == -1) return -1;
            current = "";
            // Resume from stack
            if (!working.empty()) {
                current = working.top();
                working.pop();
                cout << "Resumed task from stack: " << current << endl; 
            }
        }
    }
    doneSending = true;
    cout << "finished";
    receiveThread.join();
    return 0;
}
