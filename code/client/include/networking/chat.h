#ifndef CHAT_H
#define CHAT_H

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "sockets.h"

void chatloop();
void send_thread(int& clientSocket, std::string& username);
void listen_thread(int& clientSocket);

#endif 