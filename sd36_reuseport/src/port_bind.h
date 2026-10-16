#ifndef PORT_BIND_H
#define PORT_BIND_H
/* 성공 시 열린 소켓 fd(호출자가 close 해야 함), 실패 시 -1 */
int bind_socket_bad(int port);
int bind_socket_good(int port, const char *bind_ip);
#endif
