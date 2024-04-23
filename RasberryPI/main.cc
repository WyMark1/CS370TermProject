#include "RasberryPI.h"
#include <iostream>
#include <unistd.h>  
using namespace std;

int main() {

  while (true) {
    if (run() == -1) {
      cout << "Routing failed\n";  
    } else {
      cout << "Routing succeded\n";
    } 
  }
  return 0;
}

