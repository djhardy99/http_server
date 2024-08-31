#include <iostream>
#include <string>
#include "networking/socket_stream.h"
void chatloop(){
  std::string username;
  std::cout << "Enter a username: " << std::endl;
  std::getline(std::cin, username);
  std::cout << "Connecting to chatroom" << std::endl;  
  int client_socket = bind_socket();
  std::cout << "Connected" << std::endl; 
  const char* message = "Hello, server!";
  send_socket(client_socket,message);
}