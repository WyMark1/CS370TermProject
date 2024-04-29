#ifndef NETWORKING_H
#define NETWORKING_H
#include <string>
#include <queue> 
#include <mutex>  
#include <condition_variable>  
#include <atomic> 

template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> internalQueue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void push(const T &item) {
        std::lock_guard<std::mutex> lock(mtx);
        internalQueue.push(item);
        cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !internalQueue.empty(); });
        T item = internalQueue.front();
        internalQueue.pop();
        return item;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mtx);
        return internalQueue.empty();
    }

    int size() {
        std::lock_guard<std::mutex> lock(mtx);
        return internalQueue.size();
    }

    T front() {
        std::lock_guard<std::mutex> lock(mtx);
        return internalQueue.front();
    }
};

class Networking {
    public:
        std::string receive_ip;
        int send(int port, std::string ip, std::string data);
        std::string receive(int port);
};

void sender(ThreadSafeQueue<std::string> &sendQueue, std::atomic<bool> &doneSending, int &port, std::string &IP);
void receiver(ThreadSafeQueue<std::string> &receiveQueue, std::atomic<bool> &doneSending, int &port, Networking &net);

#endif