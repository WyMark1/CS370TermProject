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
  string data = net.receive(SERVER_PORT);
  string decrypt;
  Decrypt(data, decrypt, key);
  cout << "Data from client: "<< decrypt << '\n';
  decrypt[0] = '?'; // placeholder
  string send;
  Encrypt(decrypt,send,key);
  if (net.send(SERVER_SEND_PORT, net.receive_ip, send) == -1) return -1;
  return 0;
}