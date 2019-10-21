#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
    int rsock, wsock;
    struct sockaddr_in addr, client;
    int len;
    int ret;

    // Make socket
    rsock = socket(AF_INET, SOCK_STREAM, 0);

    if(rsock < 0) {
        fprintf(stderr, "error: Cannot make socket\n");
        return 1;
    }

    // Setting socket
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Binding socket
    ret = bind(rsock, (struct sockaddr *)&addr, sizeof(addr));

    if(ret < 0) {
        fprintf(stderr, "error: Cannot bind socket\n");
        return 1;
    }

    // Listen socket
    listen(rsock, 2);

    // Accept TCP connection from client
    len = sizeof(client);
    wsock = accept(rsock, (struct sockaddr *)&client, &len);

    // Send message
    write(wsock, "HTTP1.1 200 OK", 14);

    // Close TCP session
    close(wsock);
    close(rsock);

    return 0;
}
