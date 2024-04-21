#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.h"

#define SERVER_PORT 8080 // Updated to match Raspberry Pi client port
#define RPI_IP_ADDRESS "127.0.0.1" // Replace with the actual IP address of your Raspberry Pi

// Function to receive data from a socket
int receive_data(int sockfd, char* buffer, int buffer_size) {
  int received_bytes = recv(sockfd, buffer, buffer_size, 0);
  if (received_bytes < 0) {
    perror("recv failed");
    return -1;
  }
  return received_bytes;
}

// Function to send data to a socket
int send_data(int sockfd, const char* data, int data_length) {
  int sent_bytes = send(sockfd, data, data_length, 0);
  if (sent_bytes < 0) {
    perror("send failed");
    return -1;
  }
  return sent_bytes;
}

// Function to handle communication with the Raspberry Pi
int handle_raspberry_pi(int sockfd) {
  int file_size;

  // Receive the file size from the Raspberry Pi
  if (receive_data(sockfd, (char*)&file_size, sizeof(file_size)) <= 0) {
    return -1; // Error receiving file size
  }

  // Allocate buffer to hold the file content
  char* buffer = new char[file_size];

  // Receive the file content from the Raspberry Pi
  int received_bytes = receive_data(sockfd, buffer, file_size);
  if (received_bytes <= 0) {
    delete[] buffer;
    return -1; // Error receiving file content
  }

  // Process the received data


  // Simulate processing (can remove this later)
  sleep(2); // Simulate processing time

  // Send a response message back to the Raspberry Pi (optional)
  const char* response_message = "Data processing completed by the server!";
  send_data(sockfd, response_message, strlen(response_message));

  delete[] buffer;

  return 0;
}

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
  server_addr.sin_addr.s_addr = inet_addr(RPI_IP_ADDRESS);

  // Connect to Raspberry Pi
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connection to Raspberry Pi failed");
    exit(EXIT_FAILURE);
  }

  printf("Connected to Raspberry Pi at %s:%d\n", RPI_IP_ADDRESS, SERVER_PORT);

  // Main loop for communication with Raspberry Pi
  while (1) {
    if (handle_raspberry_pi(sockfd) < 0) {
      perror("Error handling Raspberry Pi communication");
      break;
    }
  }

  // Close the socket
  close(sockfd);

  return 0;
}
