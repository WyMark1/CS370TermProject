#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <cstring> 
#include <thread>
#include <unistd.h>  
#include <mutex>
#include <atomic>
#include <unistd.h>
#include "Client.h"
#include "../Networking/Networking.h"
#include "../Encryption/Encryption.h"

using namespace std;

int run() {
  int client_send_port = 8081;
  int client_receive_port = 8089;
  string RPI_ip_addr = "127.0.0.0"; //Replace with Raspberry Pi's IP
  string filename;

  cout << "Enter the name of the text file to send: ";
  getline(cin, filename);

  ifstream file(filename, ios::binary);
  if (!file.is_open()) {
    perror("Failed to open file");
    return -1;
  }

  ThreadSafeQueue<string> sendQueue;
  ThreadSafeQueue<string> receiveQueue;
  atomic<bool> doneSending(false);
  Networking net;
  thread sendThread(sender, ref(sendQueue), ref(doneSending), ref(client_send_port), ref(RPI_ip_addr));
  thread receiveThread(receiver, ref(receiveQueue), ref(doneSending), ref(client_receive_port), ref(net));

  string data;
  string key = "SUPER secret key";
  string output;
  while (!file.eof()) {
    getline(file, data);
    if (!file) {
      perror("Error reading file");
      return -1;
    }

    string send;
    cout << "sent " << data << "\n";
    Encrypt(data,send,key);
    sendQueue.push(send);
  }
  sleep(3);
  cout << "End of loop\n";
  while (!receiveQueue.empty()) {
    string receivedData = receiveQueue.pop();
    string decryptedData;
    Decrypt(receivedData, decryptedData, key);
    output += decryptedData;
    cout << receiveQueue.size();
  }
  doneSending = true;
  cout << "size of receive: "<< receiveQueue.size() << "\n";
  sendThread.join();
  receiveThread.join();
  cout << "size of receive: "<< receiveQueue.size() << "\n";

  cout << "Data: \n" << output << '\n';

  return 0;
}