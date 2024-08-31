#include <iostream>
#include <cstring>      // memset
#include <unistd.h>     // close
#include <sys/types.h>  // socket types
#include <sys/socket.h> // socket functions
#include <netinet/in.h> // sockaddr_in and htons
#include <arpa/inet.h>  // inet_addr
#include <stdexcept> // Runtime exceptiosn
int bind_socket(){
    const int PORT = 8080;

    // Create a socket
    const int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        throw std::runtime_error("Error: Unable to create socket");
    }

    // Prepare the sockaddr_in structure
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Use the server's IP address
    // Connect to the server
    const int connect_status_code = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (connect_status_code < 0) {
        throw std::runtime_error("Error: Unable to connect to server");
    }
    return clientSocket;
}
void send_socket(int& clientSocket,const char* message) {
    // Send a message to the server
    const int send_status_code = send(clientSocket, message, strlen(message), 0);
    if (send_status_code < 0) {
        throw std::runtime_error("Error: Unable to create socket");
    }
}
void listen_socket(){

}
void close_socket(int& clientSocket){
    const char* message = ":logoff:";
    send_socket(clientSocket,message);
    close(clientSocket);
}