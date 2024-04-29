#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstring> 
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Networking.h"

#define BUFFER_SIZE 1024 //Might want to change later
using namespace std;

int Networking::send(int port, string send_ip, string data) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(send_ip.c_str());
    addr.sin_port = htons(port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock < 0) {
        perror("socket creation failed");
        close(sock);
        return -1;
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    if (connect(sock, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("connection failed");
        close(sock);
        return -1;
    }

    int sent_bytes = ::send(sock, data.c_str(), data.size(), 0);

    if (sent_bytes < 0) {
        perror("send failed");
        close(sock);
        return -1;
    }

    close(sock);
    return 1;
}

string Networking::receive (int port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    if (bind(sock, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
      perror("Bind failed");
      close(sock);
      exit(EXIT_FAILURE);
    }

    if (listen(sock, 3) < 0) {
      perror("Listen failed");
      close(sock);
      exit(EXIT_FAILURE);
    }

    printf("Listening connections on port %d...\n", port);

    int len = sizeof(addr);
    int sock_data = accept(sock, reinterpret_cast<struct sockaddr *>(&addr), reinterpret_cast<socklen_t *>(&len));
    close(sock);
    if (sock_data < 0) {
      perror("Accept failed");
      close(sock_data);
      exit(EXIT_FAILURE);
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
    receive_ip = ip;
    printf("Connection from %s\n", ip);

    char buffer[BUFFER_SIZE];
    int received_bytes = recv(sock_data, buffer, BUFFER_SIZE, 0);
    if (received_bytes < 0) {
        perror("Receive Failed");
        close(sock_data);
        exit(EXIT_FAILURE);
    }

    printf("Received : %d bytes of data\n", received_bytes);

    close(sock_data);
    return string(buffer, received_bytes);
}

void sender(ThreadSafeQueue<string> &sendQueue, atomic<bool> &doneSending, int &port, string &IP) {
    Networking net;
    while (!doneSending) {
        if (!sendQueue.empty()) {
            string sendData = sendQueue.pop();
            net.send(port, IP, sendData);
        }
    }
}

void receiver(ThreadSafeQueue<string> &receiveQueue, atomic<bool> &doneSending, int &port, Networking &net) {
    while (!doneSending) {
        string receivedData = net.receive(port);
        if (!receivedData.empty()) {
            receiveQueue.push(receivedData);
        }
    }
}
