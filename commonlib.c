#define _GNU_SOURCE
#include "commonlib.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

void error_exit(char *msg)
{
    fprintf(stderr, "exiting due to error %s: %s\n", msg, strerror(errno));
    exit(1);
}

void print_error(char *msg)
{
    fprintf(stderr, "error: %s errno: %s\n", msg, strerror(errno));
}

int register_sigaction(int sig, const void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

void bind_port(int socket, int port)
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = (in_port_t)htons(port);
    address.sin_addr.s_addr = htons(INADDR_ANY);

    int reuse = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
        error_exit("can't enable reuse option on port");
    if (bind(socket, (struct sockaddr *)&address, sizeof(address)) == -1)
        error_exit("can't bind to socket");
}

void send_message(int socket, char *msg)
{
    if (send(socket, msg, strlen(msg), 0) == -1)
        print_error("problem sending message to client");
}

void create_response(char *response, size_t response_size)
{
    char *response_header = "HTTP/1.1 200 OK\r\n"
                            "Connection: close\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-length: 37 \r\n\r\n";
    time_t timer;
    time(&timer);
    const char *current_time = ctime(&timer);
    int l = snprintf(response, response_size, "%slocal_time: %s", response_header, current_time);
    if (l > response_size)
        print_error("buffer length too long");
}