#ifndef SERVER_H
#define SERVER_H

int receive_data(int sockfd, char* buffer, int buffer_size);
int send_data(int sockfd, const char* data, int data_length);
int handle_client(int client_sock);

#endif