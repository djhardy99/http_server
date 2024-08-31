#include <iostream>
#include <cstring>      // memset
#include <unistd.h>     // close
#include <sys/types.h>  // socket types
#include <sys/socket.h> // socket functions
#include <netinet/in.h> // sockaddr_in and htons
#include <arpa/inet.h>  // inet_addr

int main() {
    int PORT = 8080;

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: Unable to create socket" << std::endl;
        return 1;
    }

    // Prepare the sockaddr_in structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error: Unable to bind socket" << std::endl;
        close(sockfd);
        return 1;
    }
    std::cout << "Socket successfully bound to port " << PORT << std::endl;

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        std::cerr << "Error: Unable to listen on socket" << std::endl;
        close(sockfd);
        return 1;
    }
    
    // Accept a client connection
    int clientSocket = accept(sockfd, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "Error: Unable to accept client connection" << std::endl;
        close(sockfd);
        return 1;
    }
    
    // Receive a message from the client
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer)); // Clear the buffer
    if (recv(clientSocket, buffer, sizeof(buffer) - 1, 0) < 0) {
        std::cerr << "Error: Unable to receive message" << std::endl;
    } else {
        std::cout << "Message from client: " << buffer << std::endl;
    }

    // Close the sockets
    close(clientSocket);
    close(sockfd);

    return 0;
}
