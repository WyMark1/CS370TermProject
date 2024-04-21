#include "Client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CLIENT_PORT 8081 // Updated to match Raspberry Pi port

// Replace with the actual IP address of your Raspberry Pi
#define RPI_IP_ADDRESS "127.0.0.1"

int Client::send_data(const char* filename) {
  // Open the file
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    perror("Failed to open file");
    return -1;
  }

  // Get file size
  file.seekg(0, std::ios::end);
  int file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  // Allocate buffer for file contents
  char* buffer = new char[file_size];

  // Read file contents into buffer
  if (!file.read(buffer, file_size)) {
    perror("Failed to read file");
    delete[] buffer;
    return -1;
  }

  // Close the file
  file.close();

  // Connect to Raspberry Pi
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    delete[] buffer;
    return -1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(CLIENT_PORT);
  server_addr.sin_addr.s_addr = inet_addr(RPI_IP_ADDRESS);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connection failed");
    delete[] buffer;
    return -1;
  }

  // Send file size first
  int sent_bytes = send(sockfd, &file_size, sizeof(file_size), 0);
  if (sent_bytes < 0) {
    perror("Error sending file size");
    delete[] buffer;
    return -1;
  }

  // Send the file contents
  sent_bytes = send(sockfd, buffer, file_size, 0);
  if (sent_bytes < 0) {
    perror("send failed");
    delete[] buffer;
    return -1;
  }

  // Receive response from Raspberry Pi (optional)
  int received_bytes = recv(sockfd, buffer, sizeof(buffer), 0);
  if (received_bytes > 0) {
    buffer[received_bytes] = '\0'; // Ensure null termination
    printf("Raspberry Pi response: %s\n", buffer);
  } else if (received_bytes == 0) {
    printf("Raspberry Pi disconnected\n");
  } else {
    perror("Error receiving response");
  }

  close(sockfd);
  delete[] buffer;

  return 0;
}

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


