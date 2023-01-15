#include "commonlib.h"

#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>

int listener_socket;

void interrupt_handler(int sig)
{
    if (listener_socket)
        close(listener_socket);
    fprintf(stderr, "\nunexpected interrupt shutting down server\n");
    exit(0);
}

int main()
{
    if (register_sigaction(SIGINT, interrupt_handler) == -1)
        error_exit("unable to register interrupt handler");
    listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_socket == -1)
        error_exit("unable to open socket");

    bind_port(listener_socket, 3000);

    if (listen(listener_socket, 10) == -1)
        error_exit("can't listen to incoming calls");
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    puts("listening on port 3000");
    while (1)
    {
        int client_socket = accept(listener_socket, (struct sockaddr *)&client_addr, &address_size);
        if (client_socket == -1)
            error_exit("cannot open connection to client");
        // create a child process to actually work with the incoming connection
        if (!fork()) // if we are the child process, this will return 0
        {
            // close listener port, as we won't need that in the child process
            close(listener_socket);

            char response[135];
            create_response(response, sizeof(response));
            send_message(client_socket, response);

            close(client_socket);
            exit(0);
        }
        // this means we are the parent process and don't need the client connection socket
        close(client_socket);
    }
    close(listener_socket);
    return 0;
}
