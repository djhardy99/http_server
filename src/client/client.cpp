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
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error: Unable to create socket: " << clientSocket << std::endl;
        return 1;
    }

    // Prepare the sockaddr_in structure
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Use the server's IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error: Unable to connect to server" << std::endl;
        close(clientSocket);
        return 1;
    }

    // Send a message to the server
    const char* message = "Hello, server!";
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        std::cerr << "Error: Unable to send message" << std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
