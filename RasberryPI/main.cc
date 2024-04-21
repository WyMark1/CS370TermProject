#include "RasberryPI.h"
#include <iostream>

int main() {
  char route_choice;

  while (true) {
    // Prompt user for route selection
    std::cout << "Enter route to test (c for client, s for server): ";
    std::cin >> route_choice;

    // Validate user input
    while (route_choice != 'c' && route_choice != 's') {
      std::cout << "Invalid input. Please enter 'c' for client or 's' for server: ";
      std::cin.clear();
      std::cin.ignore(256, '\n');
      std::cin >> route_choice;
    }

    // Call route function based on user selection
    int result = route(route_choice);

    if (result == 0) {
      std::cout << "Route successful.\n";
    } else {
      std::cout << "Error during route.\n";
    }

    // Optionally, add a prompt to continue or exit
    char continue_choice;
    std::cout << "Continue (y/n)? ";
    std::cin >> continue_choice;
    if (continue_choice != 'y' && continue_choice != 'Y') {
      break;
    }
  }

  return 0;
}

