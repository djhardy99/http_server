#include <iostream>
#include <string>
#include "networking/socket_stream.h"
void chatloop(){
  std::string username;
  std::cout << "Enter a username: " << std::endl;
  std::getline(std::cin, username);
  std::cout << "Connecting to chatroom" << std::endl;  
  int clientSocket = bind_socket();
  std::cout << "Connected" << std::endl; 
  std::string input;
  while(true){
    std::getline(std::cin, input);
    if (input == ":q"){
      break;
    }
    const char* message = input.c_str();
    send_socket(clientSocket,message);
  }
  std::cout << "Quitting server" << std::endl;
  close_socket(clientSocket);
}
void send_thread(){

}
void listen_thread(){

}