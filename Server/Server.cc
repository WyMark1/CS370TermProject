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
    bool sent = false;
    while (true) {
        sleep(1);
        int size = receiveQueue.size();
        if (size > 0 && current.empty()) {
            string decrypt;
            string encrypted = receiveQueue.pop();
            Decrypt(encrypted, decrypt, key);
            current = decrypt;
            //do work on current
        /*} else if (size > 0 && !current.empty()) {
            //check if the burst duration is less on the next item in the queue
            // if so then push current onto the working stack if not then push the next queue value onto the stack
            //do work on current*/
        } else if (!current.empty()) { // And the burst duration is done send to the PI
            string send;
            cout << "sent " << current << "\n";
            Encrypt(current, send, key);
            if(net.send(SERVER_SEND_PORT, net.receive_ip, send) == -1) return -1;
            current = "";
            sent = true;
        } else if (current.empty() && size == 0 && sent) break;
    }
    doneSending = true;
    cout << "finished";
    receiveThread.join();
    return 0;
}
