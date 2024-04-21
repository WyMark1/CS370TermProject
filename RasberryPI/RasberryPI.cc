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
#include "RasberryPI.h"

#define CLIENT_PORT 8081  // Port for Raspberry Pi to act as server (client connection)
#define SERVER_PORT 8080  // Port for actual Server
#define BUFFER_SIZE 1024

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

// Function to handle communication (acting as server for client)
int handle_client(int client_sock) {
  while (1) {
    char buffer[BUFFER_SIZE];
    int received_bytes = receive_data(client_sock, buffer, BUFFER_SIZE);
    if (received_bytes < 0) {
      perror("Error receiving data from client");
      return -1;
    } else if (received_bytes == 0) {
      printf("Client disconnected.\n");
      break;
    }

    // Process the received data from the client (needs modified)
    printf("Received data from client: %s\n", buffer);

    // Simulate processing (can remove this later)
    sleep(2);

    // Send a response message back to the client (optional)
    const char* response_message = "Data processing completed by the RasberryPI";
    send_data(client_sock, response_message, strlen(response_message));
  }

  return 0;
}

// Function to handle communication (acting as client to server)
int handle_server(const char* server_ip, int server_port) {
  struct sockaddr_in server_addr;
  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0) {
    perror("server socket creation failed");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(server_ip);
  server_addr.sin_port = htons(server_port);

  if (connect(server_sock, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
    perror("connection to server failed");
    return -1;
  }

  // Send data to the server (you can modify this)
  const char* data_to_server = "Data from Raspberry Pi!";
  send_data(server_sock, data_to_server, strlen(data_to_server));

  // Receive response from server (optional)
  char buffer[BUFFER_SIZE];
  int received_bytes = receive_data(server_sock, buffer, BUFFER_SIZE);
  if (received_bytes > 0) {
    printf("Received data from server: %s\n", buffer);
  }

  close(server_sock);
  return 0;
}

// Function to determine destination (client or server) and handle communication
int route(char dest) {
  struct sockaddr_in client_addr;

  // Create a socket for communication with the chosen destination
  if (dest == 'c') {
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
      perror("client socket creation failed");
      exit(EXIT_FAILURE);
    }

    // Server address setup (Raspberry Pi acts as server for client)
    client_addr.sin_family = AF_INET;
        client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(CLIENT_PORT);

    // Bind the socket
    if (bind(client_sock, reinterpret_cast<struct sockaddr *>(&client_addr), sizeof(client_addr)) < 0) {
      perror("client bind failed");
      exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(client_sock, 3) < 0) {
      perror("client listen");
      exit(EXIT_FAILURE);
    }

    printf("Listening for client connections on port %d...\n", CLIENT_PORT);

    // Accept a connection from the client
    int client_len = sizeof(client_addr);
    client_sock = accept(client_sock, reinterpret_cast<struct sockaddr *>(&client_addr), reinterpret_cast<socklen_t *>(&client_len));
    if (client_sock < 0) {
      perror("client accept failed");
      exit(EXIT_FAILURE);
    }

    // Extract client IP and port from the accepted connection
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client_addr.sin_port);

    printf("Client connected from %s:%d\n", client_ip, client_port);

    // Handle communication with the connected client
    int result = handle_client(client_sock);
    close(client_sock);
    return result;

  } else if (dest == 's') {
    // Get server IP address from user (optional)
    // char server_ip[INET_ADDRSTRLEN];
    // printf("Enter server IP address: ");
    // scanf("%s", server_ip);

    // Use a pre-defined server IP address (replace with actual server IP)
    const char* server_ip = "192.168.1.X"; // Replace with your server IP

    // Handle communication with the server
    return handle_server(server_ip, SERVER_PORT);
  } else {
    printf("Invalid destination. Please enter 'c' for client or 's' for server.\n");
    return -1;
  }
}

int main() {
  char destination;

  // Get user input for destination (client or server)
  printf("Enter destination (c for client, s for server): ");
  scanf(" %c", &destination);

  // Handle communication based on user choice
  int result = route(destination);
  if (result < 0) {
    printf("Error occurred during communication.\n");
  }

  return 0;
}