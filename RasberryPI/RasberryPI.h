#ifndef RASPBERRYPI_H
#define RASPBERRYPI_H

#define BUFFER_SIZE 1024

// Function prototypes for clarity (assuming they're defined elsewhere)
int receive_data(int sockfd, char* buffer, int buffer_size);
int send_data(int sockfd, const char* data, int data_length);

int handle_client(int client_sock);
int handle_server(const char* server_ip, int server_port);

int route(char dest);

#endif
