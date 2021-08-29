#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080


int main(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr, "Could not create socket: %s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Could not bind socket %d: %s\n", server_fd, strerror(errno));
        exit(1);
    }

    if (listen(server_fd, 10) < 0) {
        fprintf(stderr, "Could not listen to socket %d: %s\n", server_fd, strerror(errno));
        exit(1);
    }
    printf("Listening on port %d...\n", PORT);

    while(1) {
        struct sockaddr_in client_addr = {0};
        socklen_t client_addr_len = 0;

        int client_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_addr_len);
        printf("Heard something!\n");
        if(client_fd < 0) {
            fprintf(stderr, "Could not accept incoming connection: %s\n", strerror(errno));
            exit(1);
        }

        char hypertext[] = "<html><head><title>YO</title></head><body>This is a C HTTP server response</body></html>";

        dprintf(client_fd, "HTTP/1.1 200 OK\r\n");
        dprintf(client_fd, "Content-Type: text/html; charset=charset=ASCII\r\n");
        dprintf(client_fd, "Server: C HTTP Server\r\n");
        dprintf(client_fd, "Content-Length: %lu\r\n", strlen(hypertext));
        dprintf(client_fd, "\r\n");
        dprintf(client_fd, "%s\r\n", hypertext);

        if(close(client_fd) < 0) {
            fprintf(stderr, "Could not close the client socket %d: %s\n", client_fd, strerror(errno));
            exit(1);
        }
    }

    return 0;
}

