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

#define SERVER_IP_ADDRESS "192.168.1.Y" // Replace Y with the actual IP addess of the server
#define CLIENT_IP_ADDRESS "192.168.1.Y" // Replace Y with the actual IP addess of the client
#define CLIENT_PORT 8081
#define SERVER_PORT 8080

using namespace std;

//This is completely untested and will almost certianly have to be fixed
int route(char dest) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024] = {0};
    int client_port, server_port;
    const char *ip;
    if (dest == 's') {
        client_port = CLIENT_PORT;
        server_port = SERVER_PORT;
        ip = SERVER_IP_ADDRESS;
    } else if (dest == 'c') {
        client_port = SERVER_PORT;
        server_port = CLIENT_PORT;
        ip = CLIENT_IP_ADDRESS;
    }

    // Create socket for server
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(client_port);

    // Bind server socket
    if (bind(server_sock, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(server_sock, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(client_addr);
    
    // Accept client connection
    if ((client_sock = accept(server_sock, reinterpret_cast<struct sockaddr *>(&client_addr), reinterpret_cast<socklen_t*>(&addrlen))) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Create socket for server connection
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(ip);

    // Connect to server
    if (connect(server_socket, reinterpret_cast<struct sockaddr *>(&server), sizeof(server)) < 0) {
        perror("connection to server failed");
        exit(EXIT_FAILURE);
    }

    // Start routing data between client and server
    int valread;
    while ((valread = read(client_sock, buffer, 1024)) > 0) {
        // Edit the file on the Raspberry Pi before forwarding to the server
        //editFile(buffer, valread, "edited_file.txt");

        // Forward the edited data to the server
        send(server_socket, buffer, valread, 0);

        memset(buffer, '\0', sizeof(buffer)); // Clear buffer after each read
    }

    close(client_sock);
    close(server_socket);
    return 0;
}

int runner() {
    while (true) {
        route('s'); // Change in the in future to parallize this on thread dose s and one does c
        route('c');
    }
}