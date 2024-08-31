#include "networking/chat.h"

std::atomic<bool> keepRunning(true); 

void send_thread(int& clientSocket, std::string& username) {
    std::string input;
    while (keepRunning) {
        std::getline(std::cin, input);
        if (input == ":q") {
            keepRunning = false;  // Signal to stop other thread
            break;
        }
        std::string send_string = username + " : " + input;
        const char* message = send_string.c_str();
        try {
            send_socket(clientSocket, message);
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            keepRunning = false;
        }
    }
}

void listen_thread(int& clientSocket) {
    char buffer[1024];
    while (keepRunning) {
        ssize_t bytes_received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate the buffer
            std::cout << "Received: " << buffer << std::endl;
        } else if (bytes_received == 0) {
            std::cout << "Connection closed by server." << std::endl;
            keepRunning = false;
        } else {
            std::cerr << "Error receiving data." << std::endl;
            keepRunning = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep to prevent busy-waiting
    }
}

void chatloop() {
    std::string username;
    std::cout << "Enter a username: " << std::endl;
    std::getline(std::cin, username);

    // Connect to the server
    int clientSocket = bind_socket();  // This should probably be a connect_socket function
    std::cout << "Connected" << std::endl;

    // Start threads
    std::thread sendThread(send_thread, std::ref(clientSocket), std::ref(username));
    std::thread listenThread(listen_thread, std::ref(clientSocket));

    // Await join
    sendThread.join();
    listenThread.join();

    std::cout << "Quitting chat" << std::endl;
    close_socket(clientSocket);
}
