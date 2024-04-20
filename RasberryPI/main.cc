#include <iostream>
#include "RasberryPI.h"

int main() {
  // Test routing to server (simulated)
  std::cout << "Testing route to server (simulated):\n";
  int result = route('s');
  if (result == 0) {
    std::cout << "  Route to server successful (simulated).\n";
  } else {
    std::cout << "  Error during route to server (simulated).\n";
  }

  // Test routing to client (simulated)
  std::cout << "\nTesting route to client (simulated):\n";
  result = route('c');
  if (result == 0) {
    std::cout << "  Route to client successful (simulated).\n";
  } else {
    std::cout << "  Error during route to client (simulated).\n";
  }

  return 0;
}