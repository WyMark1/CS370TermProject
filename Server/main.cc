#include "Server.h"
#include <iostream>
#include <unistd.h>
using namespace std;

int main() {

  while (true) {
    if (run() == -1) {
      cout << "Server send failed\n";  
    } else {
      cout << "Server send succeded\n";
    } 
  }
  return 0;
}

