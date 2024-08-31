#ifndef CHAT_H
#define CHAT_H
int chatloop();
void send_thread(int& clientSocket);
void listen_thread(int& clientSocket);
#endif // CHAT_H