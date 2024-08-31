#include "networking/server.h"


void connectionsHandler::remove_client_from_list(int& clientSocket) {
    std::lock_guard<std::mutex> guard(clientSetMutex);
    clientSet.erase(clientSocket);
}

void connectionsHandler::close_all_clients() {
    std::lock_guard<std::mutex> guard(clientSetMutex);
    for (const int& clientSocket : clientSet) {
        close(clientSocket);
    }
    clientSet.clear(); // Clear the set after closing all sockets
}

// Existing functions
void run_server() {
    std::atomic<bool> keepRunning(true);
    connectionsHandler conn_handler(&keepRunning);

    // Bind a socket
    int serverSocket = bind_socket();

    // Configure the socket to listen
    const int MAX_QUEUE = 5;
    if (listen(serverSocket, MAX_QUEUE) < 0) {
        close(serverSocket);
        throw std::runtime_error("Error: Unable to listen on socket");
    }

    // Start the accept thread
    std::thread acceptThread(accept_thread, std::ref(serverSocket), std::ref(keepRunning), std::ref(conn_handler));
    acceptThread.join();

    std::cout << "Closing server" << std::endl;
    conn_handler.close_all_clients();
    close(serverSocket);
    std::cout << "Done" << std::endl;
}

void accept_thread(int& serverSocket, std::atomic<bool>& keepRunning, connectionsHandler& conn_handler) {
    while (keepRunning) {
        int clientSocket = accept_socket(serverSocket);
        if (clientSocket >= 0) {
            conn_handler.clientSet.insert(clientSocket); // Add client to the list
            std::thread clientThread(listen_thread, clientSocket, std::ref(conn_handler));
            clientThread.detach(); // Detach the thread to handle multiple clients
        }
    }
}

void listen_thread(int clientSocket, connectionsHandler& conn_handler) {
    char buffer[1024];
    while (conn_handler.keepRunning->load()) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer

        ssize_t received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (received < 0) {
            std::cerr << "Error: Unable to receive message" << std::endl;
            continue;
        }
        if (received == 0) {
            std::cout << "(Connection closed) Client " << clientSocket << std::endl;
            conn_handler.remove_client_from_list(clientSocket);
            close_socket(clientSocket);
            return;
        }

        buffer[received] = '\0'; // Null-terminate the buffer
        std::cout << "(Received) " << buffer << std::endl;

        if (buffer[0] == ':') {
            std::cout << "(Received) Logoff from client " << clientSocket << std::endl;
            std::string logoff_str = "Log off occurred";
            send_received_message(conn_handler, logoff_str.c_str(), clientSocket);
            conn_handler.remove_client_from_list(clientSocket);
            close_socket(clientSocket);
            return;
        }

        // Resend message to all other clients
        send_received_message(conn_handler, buffer, clientSocket);
    }
}

void send_received_message(connectionsHandler& conn_handler, const char* buffer, int senderSocket) {
    std::lock_guard<std::mutex> guard(conn_handler.clientSetMutex);
    for (const int& client : conn_handler.clientSet) {
        if (client != senderSocket) {
            ssize_t send_status_code = send(client, buffer, strlen(buffer), 0);
            if (send_status_code < 0) {
                std::cerr << "Error: Unable to send message" << std::endl;
            }
        }
    }
}

int bind_socket() {
    const int PORT = 8080;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Error: Unable to create socket");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Error: Unable to bind socket");
    }
    std::cout << "Socket successfully bound to port " << PORT << std::endl;
    return serverSocket;
}

int accept_socket(int& serverSocket) {
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "[server.accept_socket] - Error, unable to accept a client connection" << std::endl;
    }
    return clientSocket;
}

void close_socket(int& clientSocket) {
    close(clientSocket);
}
