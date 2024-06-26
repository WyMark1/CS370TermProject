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
    size_t loc = data.find("BurstTImePI");
    size_t start = loc + string("BurstTImePI").length();
    string burstTimeStr = data.substr(0, start);
    size_t burstTime = stoi(burstTimeStr);
    return burstTime;
}

string removeBurst(string str) {
    size_t loc = str.find("BurstTImePI");
    str.erase(0, loc+string("BurstTImePI").length());
    return str;
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
        this_thread::sleep_for(chrono::milliseconds(3));
        int size = receiveQueue.size();
        if (current.empty() && currVal > 0) {
            string send;
            string val = to_string(currVal);
            cout << "sent " << currVal << "\n";
            Encrypt(val, send, key);
            this_thread::sleep_for(chrono::milliseconds(1));
            if(net.send(SERVER_SEND_PORT, net.receive_ip, send) == -1) return -1;
            current = "";
            currVal = 0;
        } else if (size > 0 && current.empty()) {
            string encrypted;
            if (!working.empty()) {
                if (working.front().length() <  getBurstTime(receiveQueue.front())) {
                    current = working.front();
                    currVal = workingVals.front();
                    working.pop();
                    workingVals.pop();
                }
            } 

            if (current.empty()) {
                encrypted = receiveQueue.pop();
                encrypted = removeBurst(encrypted);
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
                working.push(current);
                workingVals.push(currVal);
                current = receiveQueue.pop();
                current = removeBurst(current);
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
            cout << "Current: "<< current << "\n";
            currVal += current[0];
            current.erase(0, 1);
        }
    }

    doneSending = true;
    cout << "finished";
    receiveThread.join();
    return 0;
}