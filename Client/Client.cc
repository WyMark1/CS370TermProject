#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstring> 
#include <unistd.h>
#include "Client.h"
#include "../Networking/Networking.h"
#include "../Encryption/Encryption.h"

#define CLIENT_PORT 8081 // Client port
#define CLIENT_SEND_PORT 8089
#define RPI_IP_ADDRESS "127.0.0.0" // Replace with Raspberry Pi's IP
#define BUFFER_SIZE 1024
using namespace std;

int run() {
  string filename;

  cout << "Enter the name of the text file to send: ";
  getline(cin, filename);

  ifstream file(filename, ios::binary);
  if (!file.is_open()) {
    perror("Failed to open file");
    return -1;
  }

  char* buffer = new char[BUFFER_SIZE];
  string key = "SUPER secret key";
  string output;
  Networking net;
  while (!file.eof()) {
    memset(buffer, 0, BUFFER_SIZE); //Delete what is in the buffer
    int bytes_read = file.read(buffer, BUFFER_SIZE).gcount();
    if (bytes_read < 0) {
      perror("Error reading file");
      delete[] buffer;
      return -1;
    }
    string data = string(buffer, BUFFER_SIZE);
    string send;
    Encrypt(data,send,key);
    if (net.send(CLIENT_PORT,RPI_IP_ADDRESS, send) == -1) {
      delete[] buffer;
      return -1;
    } else {
      data = net.receive(CLIENT_SEND_PORT);
      Decrypt(data,send,key);
      output += send;
    }
  }
  delete[] buffer;
  cout << "Data: \n" << output << '\n';

  return 0;
}