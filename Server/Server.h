#ifndef SERVER_H
#define SERVER_H

// Function prototypes for server functionality
int receive_data(int sockfd, char* buffer, int buffer_size); // Returns the number of bytes received (or -1 on error)
int send_data(int sockfd, const char* data, int data_length); // Returns the number of bytes sent (or -1 on error)
int handle_client(int client_sock); // Returns 0 on success, -1 on error

#endif
