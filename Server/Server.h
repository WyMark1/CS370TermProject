#ifndef SERVER_H
#define SERVER_H

// Function prototypes for server functionalities
int receive_data(int sockfd, char* buffer, int buffer_size);
int send_data(int sockfd, const char* data, int data_length);
int handle_raspberry_pi(int sockfd);

#endif

