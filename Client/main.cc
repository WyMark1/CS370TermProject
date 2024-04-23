#include "Client.h"
#include <iostream>
#include <unistd.h>
using namespace std;

int main() {

  while (true) {
    if (run() == -1) {
      cout << "Client send failed\n";  
    } else {
      cout << "Client send succeded\n";
    } 
  }
  return 0;
}

