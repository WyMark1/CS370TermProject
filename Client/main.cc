#include <iostream>
#include "Client.h"

int main() {
  std::string filename;
  
  // Prompt user to enter filename
  std::cout << "Enter the name of the text file to send: ";
  std::getline(std::cin, filename);

  // Create a Client object
  Client client;

  // Send the file contents to the server
  if (client.send_data(filename.c_str()) < 0) {
    std::cerr << "Error sending file to server" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "File sent successfully!" << std::endl;

  return EXIT_SUCCESS;
}

