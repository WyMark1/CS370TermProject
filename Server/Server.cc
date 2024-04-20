#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
  char buffer[1024];
  int received_bytes;

  // Receive data from client
  received_bytes = receive_data(client_sock, buffer, sizeof(buffer));
  if (received_bytes <= 0) {
    return -1; // Error or client disconnected
  }

  // Process the received data (replace with your logic)
  printf("Received data from client: %s\n", buffer);

  // Simulate processing (can remove this later)
  sleep(2); // Simulate processing time

  // Send processed data back to client (replace with actual processed data)
  const char* processed_data = "Data processed by server!";
  send_data(client_sock, processed_data, strlen(processed_data));

  return 0;
}