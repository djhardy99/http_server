#ifndef SERVER_H
#define SERVER_H

#include <atomic>
#include <cstring>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unordered_set>
#include <unistd.h>

// Forward declarations
struct connectionsHandler; 
void run_server();
void accept_thread(int& serverSocket, std::atomic<bool>& keepRunning, connectionsHandler& conn_handler);
void listen_thread(int clientSocket, connectionsHandler& conn_handler);
void send_received_message(connectionsHandler& conn_handler, const char* buffer, int senderSocket);
int bind_socket();
int accept_socket(int& serverSocket);
void close_socket(int& clientSocket);

struct connectionsHandler {
    std::unordered_set<int> clientSet;
    const int MAX_CONNECTIONS = 10;
    std::mutex clientSetMutex;
    std::atomic<bool>* keepRunning;

    connectionsHandler(std::atomic<bool>* running) : keepRunning(running) {}

    void accept_client_connections_thread(int& serverSocket);
    void remove_client_from_list(int& clientSocket);
    void close_all_clients();
};

#endif // SERVER_H
