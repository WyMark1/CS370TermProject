#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include "Server.h"
#include "../Networking/Networking.h"
#include "../Encryption/Encryption.h"

#define SERVER_PORT 8080 
#define SERVER_SEND_PORT 8083
using namespace std;

// Comparator for priority queue ordering by arrival time
struct TaskComparator {
    bool operator()(const Task& a, const Task& b) {
        return a.arrival_time > b.arrival_time;
    }
};

priority_queue<Task, vector<Task>, TaskComparator> taskQueue; 
Task activeTask;

int run() {
    Networking net;
    string key = "SUPER secret key";

    int taskCount = 0;
    double totalTurnaroundTime = 0;
    double totalWaitingTime = 0;

    while (true) { 
        string data = net.receive(SERVER_PORT);
        string decrypt;
        Decrypt(data, decrypt, key);

        // Extract burst time (assuming space-delimited format)
        size_t pos = decrypt.find(' ');
        string burstTimeStr = decrypt.substr(0, pos);
        string taskData = decrypt.substr(pos + 1);

        // Calculate arrival time and burst duration (you may need to refine this)
        int arrival_time = time(nullptr); 
        int burst_duration = stoi(burstTimeStr); // Example if burst time is an integer

        // Add new task to queue
        taskQueue.push(Task{burstTimeStr, arrival_time, burst_duration, taskData, 0});

        while (!activeTask.data.empty() || !taskQueue.empty()) {
            if (activeTask.data.empty()) {
                activeTask = taskQueue.top();
                taskQueue.pop();
            }

            // Simulate work for a short duration 
            sleep(1); 
            activeTask.progress++; 

            // Check for higher priority tasks (arriving earlier)
            if (!taskQueue.empty() && taskQueue.top().arrival_time < activeTask.arrival_time) {
                // Preemption: Save progress and switch tasks
                taskQueue.push(activeTask); 
                activeTask = taskQueue.top();
                taskQueue.pop();
            } else if (activeTask.progress == activeTask.burst_duration) {
                // Task Completion:
                int completionTime = time(nullptr);
                totalTurnaroundTime += completionTime - activeTask.arrival_time;
                totalWaitingTime += completionTime - activeTask.arrival_time - activeTask.burst_duration;
                taskCount++;

                string send;
                Encrypt(decrypt,send,key);
                if (net.send(SERVER_SEND_PORT, net.receive_ip, send) == -1) return -1;

                activeTask = {}; // Reset active task
            }
        }
    }
    return 0;
}
