#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstring> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Client.h"

#define SERVER_IP_ADDRESS "192.168.1.Y" // Replace Y with the actual IP address of your server
#define SERVER_PORT 8081

int main() {
  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Server address structure
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

  // Connect to the server
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connection failed");
    exit(EXIT_FAILURE);
  }

  // Prepare data to send
  char data[1024];
  int data_length;
  if (prepare_data(data, &data_length) < 0) {
    exit(EXIT_FAILURE);
  }

  // Send data to the server
  int sent_bytes = send_data(sockfd, data, data_length);
  if (sent_bytes < 0) {
    exit(EXIT_FAILURE);
  }

  // Receive processed data from the server
  char received_data[1024];
  int received_bytes = receive_data(sockfd, received_data, sizeof(received_data));
  if (received_bytes < 0) {
    exit(EXIT_FAILURE);
  }

  // Display received data
  printf("Received data from server: %s\n", received_data);

  // Close the socket
  close(sockfd);

  return 0;
}
