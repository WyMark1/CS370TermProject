#ifndef RASPBERRYPI_H
#define RASPBERRYPI_H

// Include necessary system headers for sockets and network communication
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Forward declarations for functions defined in raspberrypi.cc
int route(char dest);  // Function to route data based on destination (server or client)

#endif /* RASPBERRYPI_H */