#ifndef CLIENT_H
#define CLIENT_H

// Function prototypes for functions defined in Client.cc
int send_data(int sockfd, const char* data, int data_length);
int receive_data(int sockfd, char* buffer, int buffer_size);
int prepare_data(char* data, int* data_length);

#endif /* CLIENT_H */