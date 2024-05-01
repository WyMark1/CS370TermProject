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

size_t getBurstTime(string data) {
    size_t loc = data.find("Burst Time: ");
    size_t start = loc + string("Burst Time: ").length();
    string burstTimeStr = data.substr(start);
    size_t burstTime = stoi(burstTimeStr);
    return burstTime;
}

void removeBurst(string str) {
    size_t loc = str.find("Burst Time: ");
    str.erase(loc);
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
    int currVal = 0;
    queue<string> working;
    queue<int> workingVals;
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(30));
        int size = receiveQueue.size();
        if (current.empty() && currVal > 0) {
            string send;
            string val = to_string(currVal);
            cout << "sent " << currVal << "\n";
            Encrypt(val, send, key);
            this_thread::sleep_for(chrono::milliseconds(15));
            if(net.send(SERVER_SEND_PORT, net.receive_ip, send) == -1) return -1;
            current = "";
            currVal = 0;
        } else if (size > 0 && current.empty()) {
            string encrypted;
            if (!working.empty()) {
                cout << "Working: " << working.front().length() << " < receiveQueue: " << getBurstTime(receiveQueue.front()) << "\n";
                if (working.front().length() <  getBurstTime(receiveQueue.front())) {
                    current = working.front();
                    currVal = workingVals.front();
                    working.pop();
                    workingVals.pop();
                }
            } 

            if (current.empty()) {
                cout <<"receiveQueue: " << getBurstTime(receiveQueue.front()) << "\n";
                encrypted = receiveQueue.pop();
                removeBurst(encrypted);
                currVal = 0;
                string decrypt;
                Decrypt(encrypted, decrypt, key);
                current = decrypt;
            }
            cout << "Current: "<< current << "\n";
            currVal += current[0];
            current.erase(0, 1);
        } else if (size > 0 && !current.empty()) {
            if (!working.empty()) {
                cout << "Workingl: " << working.front().length() << " < receiveQueue: " << getBurstTime(receiveQueue.front()) << "\n";
                if (working.front().length() <  getBurstTime(receiveQueue.front())) {
                    if (current.length() > working.front().length()) {
                        working.push(current);
                        workingVals.push(currVal);
                        current = working.front();
                        currVal = workingVals.front();
                        working.pop();
                        workingVals.pop();
                    }
                } 
            } 
            if (current.length() > getBurstTime(receiveQueue.front())) {
                cout <<"receiveQueuel: " << getBurstTime(receiveQueue.front()) << "\n";
                working.push(current);
                workingVals.push(currVal);
                current = receiveQueue.pop();
                removeBurst(current);
                currVal = 0;
                string decrypt;
                Decrypt(current, decrypt, key);
                current = decrypt;
            }
            cout << "Current: "<< current << "\n";
            currVal += current[0];
            current.erase(0, 1);
        } else if (!current.empty()) {
            if (!working.empty()) {
                if (current.length() > working.front().length()) {
                    working.push(current);
                    workingVals.push(currVal);
                    current = working.front();
                    currVal = workingVals.front();
                    working.pop();
                    workingVals.pop();
                }
            }
            cout << "Current: "<< current << "\n";
            currVal += current[0];
            current.erase(0, 1);
        } else if (current.empty() && !working.empty()) {
            current = working.front();
            currVal = workingVals.front();
            working.pop();
            workingVals.pop();
            currVal += current[0];
            current.erase(0, 1);
        }
    }

    doneSending = true;
    cout << "finished";
    receiveThread.join();
    return 0;
}
