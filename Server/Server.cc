#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "Server.h"
#include "../Networking/Networking.h"
#include "../Encryption/Encryption.h"

#define SERVER_PORT 8080 
#define SERVER_SEND_PORT 8083
using namespace std;

int run() {
  Networking net;
  string key = "SUPER secret key";

  // Receive data from the Raspberry Pi
  string data = net.receive(SERVER_PORT);

  // Decrypt the data
  string decrypt;
  Decrypt(data, decrypt, key);

  // data processing here 
  reverse(decrypt.begin(), decrypt.end()); 

  // Encrypt the modified data
  string send;
  Encrypt(decrypt, send, key);

  // Send the response back to the Raspberry Pi
  if (net.send(SERVER_SEND_PORT, net.receive_ip, send) == -1) return -1;  

  return 0;
}