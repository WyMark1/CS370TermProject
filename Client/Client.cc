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
  string RPI_ip_addr = "129.82.44.169"; //Replace with Raspberry Pi's IP
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
  int expected = 0;
  while (!file.eof()) {
    getline(file, data);
    if (!file) {
      perror("Error reading file");
      return -1;
    }
    this_thread::sleep_for(chrono::milliseconds(1));
    string send;
    cout << "sent " << data << "\n";
    Encrypt(data,send,key);

    //add bursttime to data
    string burstTime = to_string(data.size());
    send = burstTime + "BurstTImePI" + send;

    sendQueue.push(send);
    expected++;
  }


  int i=0;
  while (i != expected) {
    if (!receiveQueue.empty()) {
      string receivedData = receiveQueue.pop();
      string decryptedData;
      Decrypt(receivedData, decryptedData, key);
      output += "\n" + decryptedData;
      cout << "Data: " << output << "\n";
      i++;
    }
  }
  doneSending = true;
  cout << "Final Data: \n" << output << '\n';

  sendThread.join();
  receiveThread.join();
  return 0;
}