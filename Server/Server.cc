#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>

#define SERVER_PORT 8080

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

// Function to handle client connection (replace with processing logic)
int handle_client(int client_sock) {
  int file_size;

  // Receive the file size from the client
  if (receive_data(client_sock, (char*)&file_size, sizeof(file_size)) <= 0) {
    return -1; // Error receiving file size
  }

  // Allocate buffer to hold the file content
  char* buffer = new char[file_size];

  // Receive the file content from the client
  int received_bytes = receive_data(client_sock, buffer, file_size);
  if (received_bytes <= 0) {
    delete[] buffer;
    return -1; // Error receiving file content
  }

  // Write the received data to a file
  std::ofstream output_file("received_file.txt", std::ios::binary);
  if (!output_file.is_open()) {
    perror("Failed to create output file");
    delete[] buffer;
    return -1;
  }

  output_file.write(buffer, file_size);
  output_file.close();

  delete[] buffer;

  printf("Received file successfully (size: %d bytes)\n", file_size);

  // Simulate processing (can remove this later)
  sleep(2); // Simulate processing time

  // Send a response message back to the client
  const char* response_message = "File received and processed by the server!";
  send_data(client_sock, response_message, strlen(response_message));

  return 0;
}
