#include "Client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <arpa/inet.h>

#define SERVER_IP_ADDRESS "192.168.1.Y" // Replace Y with the actual IP address of your server or Raspberry Pi (if acting as server)
#define SERVER_PORT 8081

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

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    delete[] buffer;
    return -1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

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

  // Receive response from server (optional)
  int received_bytes = recv(sockfd, buffer, sizeof(buffer), 0);
  if (received_bytes > 0) {
    buffer[received_bytes] = '\0'; // Ensure null termination
    printf("Server response: %s\n", buffer);
  } else if (received_bytes == 0) {
    printf("Server disconnected\n");
  } else {
    perror("Error receiving response");
  }

  close(sockfd);
  delete[] buffer;

  return 0;
}
