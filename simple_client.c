#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "commonlib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{

    while (1)
    {
        int listener = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in si;
        memset(&si, 0, sizeof(si));
        si.sin_family = PF_INET;
        si.sin_addr.s_addr = inet_addr("127.0.0.1");
        si.sin_port = htons(3000);
        if (connect(listener, (struct sockaddr *)&si, sizeof(si)) == -1)
            error_exit("cannot connect to host");
        char buf[132];
        int data = recv(listener, buf, 132, 0);
        if (data == -1)
        {
            error_exit("cannot receive message");
        }
        fprintf(stdout, "new message of %d bytes:\n%s\n", data, buf);
        close(listener);
        sleep(10);
    }
}