#ifndef COMMONLIB_H
#define COMMONLIB_H

#include <stddef.h>

// setup socket address and bind it to listener_socket
void bind_port(int socket, int port);

// send message to client, print error to stderr but don't exit program
void send_message(int socket, char *msg);

void create_response(char *response, size_t response_size);

// print error with errno string to stderr and exit with status 1
void error_exit(char *msg);

// just prints the formatted error message to stderr
void print_error(char *msg);

// registers a custom signal handler
int register_sigaction(int sig, const void (*handler)(int));

#endif