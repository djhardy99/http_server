#include <iostream>
#include <cstring>      // memset
#include <unistd.h>     // close
#include <sys/types.h>  // socket types
#include <sys/socket.h> // socket functions
#include <netinet/in.h> // sockaddr_in and htons
#include <arpa/inet.h>  // inet_addr

int main() {
    int PORT = 8080;
    int sockfd;

    // Create a socket
    // AF_INET = 2, SOCK_STREAM = 1
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: Unable to create socket" << std::endl;
        return 1;
    }

    // Prepare the sockaddr_in structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the structure
    server_addr.sin_family = AF_INET; // Address family
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces (INADDR_ANY = 0)
    server_addr.sin_port = htons(PORT); // Port to bind to (convert to network byte order)
    // Bind the socket to the address and port
    int bind_respones_code = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_respones_code < 0) {
        std::cerr << "Error: Unable to bind socket" << std::endl;
        close(sockfd);
        return 1;
    }
    std::cout << "Socket successfully bound to port "<< PORT << std::endl;

    listen(sockfd, 5); // 5 is the maximum number of client connections that can be queued
    int clientSocket = accept(sockfd, nullptr, nullptr);
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;
    // Close the socket
    close(sockfd);

    return 0;
}