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

#define CLIENT_PORT 8081 // Updated to match Raspberry Pi port
#define RPI_IP_ADDRESS "127.0.0.1" // Replace with Raspberry Pi's IP

int main() {
  std::string filename;

  // Prompt user to enter filename
  std::cout << "Enter the name of the text file to send: ";
  std::getline(std::cin, filename);

  // Open the file
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  // Get file size (optional, not used for dynamic buffer)
  // file.seekg(0, std::ios::end);
  // int file_size = file.tellg();
  // file.seekg(0, std::ios::beg);

  // Determine a dynamic buffer size based on system capabilities (optional)
  int buffer_size = 4096; // Start with a reasonable default

  #ifdef _SC_PAGESIZE
  int page_size = sysconf(_SC_PAGESIZE);
  if (page_size > 0) {
    buffer_size = page_size; // Use system page size for efficiency
  }
  #endif

  char* buffer = new char[buffer_size];

  // Connect to Raspberry Pi (server)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    delete[] buffer;
    return EXIT_FAILURE;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(CLIENT_PORT);
  server_addr.sin_addr.s_addr = inet_addr(RPI_IP_ADDRESS);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connection failed");
    delete[] buffer;
    return EXIT_FAILURE;
  }

  // Set timeout for receiving response from Raspberry Pi
  int timeout = 20; // Set a timeout in seconds (adjust as needed)
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

  // Send data to Raspberry Pi (server)
  while (!file.eof()) {
    int bytes_read = file.read(buffer, buffer_size).gcount();
    if (bytes_read < 0) {
      perror("Error reading file");
      delete[] buffer;
      close(sockfd);
      return EXIT_FAILURE;
    }

    int sent_bytes = send(sockfd, buffer, bytes_read, 0);
    if (sent_bytes < 0) {
      perror("send failed");
      delete[] buffer;
      close(sockfd);
      return EXIT_FAILURE;
    }
  }

  // Receive response from Raspberry Pi (containing processed data from server)
  char response_buffer[1024];
  int received_bytes = recv(sockfd, response_buffer, sizeof(response_buffer), 0);

  if (received_bytes > 0) {
    response_buffer[received_bytes] = '\0'; // Ensure null termination
    printf("Processed data from server: %s\n", response_buffer);
  } else if (received_bytes == 0) {
    printf("Raspberry Pi disconnected before sending response\n");
  } else {
    perror("Error receiving response");
  }

  // Close connection
  close(sockfd);
  delete[] buffer;

  return EXIT_SUCCESS;
}