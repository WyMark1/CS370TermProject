#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "Server.h"
#include "../Networking/Networking.h"

#define SERVER_PORT 8080 
using namespace std;

int run() {
  Networking net;
  string data = net.receive(SERVER_PORT);
  // decrypt data
  // do something to data
  if (net.send(SERVER_PORT, net.receive_ip, data.append(" came from server")) == -1) return -1;
  return 0;
}