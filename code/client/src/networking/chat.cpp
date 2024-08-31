#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "networking/socket_stream.h"

std::atomic<bool> keepRunning(true); 

void send_thread(int& clientSocket, std::string& username) {
    std::string input;
    while (keepRunning) {
        std::getline(std::cin, input);
        std::string send_string = username + " : " + input;
        if (input == ":q") {
            keepRunning = false;  // Signal to stop other thread
            break;
        }
        const char* message = send_string.c_str();
        send_socket(clientSocket, message);
    }
}

void listen_thread(int& clientSocket) {
    while (keepRunning) {
        // You might want to use recv_socket() or similar function to read data
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep to prevent busy-waiting
    }
}

void chatloop(){
  std::string username;
  std::cout << "Enter a username: " << std::endl;
  std::getline(std::cin, username);
  std::cout << "Connecting to chatroom" << std::endl;  
  int clientSocket = bind_socket();
  std::cout << "Connected" << std::endl; 
  std::string input;
  // Start threads
  std::thread sendThread(send_thread, std::ref(clientSocket),std::ref(username));
  std::thread listenThread(listen_thread, std::ref(clientSocket));
  // Await join
  sendThread.join();
  listenThread.join();

  std::cout << "Quitting server" << std::endl;
  close_socket(clientSocket);
}
