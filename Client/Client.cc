#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Client.h"

#define SERVER_IP_ADDRESS "192.168.1.Y" // Replace Y with the actual IP address of your server
#define SERVER_PORT 8081

int send_data(int sockfd, const char* data, int data_length) {
  int sent_bytes = send(sockfd, data, data_length, 0);
  if (sent_bytes < 0) {
    perror("send failed");
    return -1;
  }
  return sent_bytes;
}

int receive_data(int sockfd, char* buffer, int buffer_size) {
  int received_bytes = recv(sockfd, buffer, buffer_size, 0);
  if (received_bytes < 0) {
    perror("recv failed");
    return -1;
  }
  return received_bytes;
}

// Function to prepare data to send (replace with data type)
int prepare_data(char* data, int* data_length) {
  printf("Enter data to send to server: ");
  fgets(data, sizeof(data), stdin);
  *data_length = strlen(data);
  return 0;
}
