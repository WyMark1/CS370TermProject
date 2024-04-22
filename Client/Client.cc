#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstring> 
#include <unistd.h>
#include "Client.h"
#include "../Networking/Networking.h"

#define CLIENT_PORT 8081 // Client port
#define RPI_IP_ADDRESS "129.82.44.80" // Replace with Raspberry Pi's IP
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
  string output;
  Networking net;
  while (!file.eof()) {
    int bytes_read = file.read(buffer, BUFFER_SIZE).gcount();
    if (bytes_read < 0) {
      perror("Error reading file");
      delete[] buffer;
      return -1;
    }
    //Encrypt data
    if (net.send(CLIENT_PORT,RPI_IP_ADDRESS,string(buffer, BUFFER_SIZE)) == -1) {
      delete[] buffer;
      return -1;
    }

    output += net.receive(CLIENT_PORT);
  }

  cout << "Data: \n" << output << '\n';

  delete[] buffer;

  return 0;
}