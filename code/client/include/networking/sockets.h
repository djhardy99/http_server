#ifndef SOCKETS_H
#define SOCKETS_H

#include <iostream>
#include <cstring>      
#include <unistd.h>     
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <stdexcept> 

int bind_socket();
void send_socket(int& clientSocket,const char* message);
void listen_socket();
void close_socket(int& clientSocket);

#endif 