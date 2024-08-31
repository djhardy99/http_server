#ifndef SOCKET_STREAM_H
#define SOCKET_STREAM_H
int bind_socket();
void send_socket(int& clientSocket,const char* message);
void listen_socket();
void close_socket(int& clientSocket);
#endif // SOCKET_STREAM_H