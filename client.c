#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int client_socket_fd;
    struct sockaddr_in server_addr;
    char rd_buffer[128];

    client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_fd == -1) {
        error_at_line(-1, errno, __FILE__, __LINE__, "Error from socket()");

    }

    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client_socket_fd, (struct sockaddr *)&server_addr, (socklen_t)sizeof(struct sockaddr_in)) == -1){
        error_at_line(-1, errno, __FILE__, __LINE__, "Error from connect()");
    }

    if(read(client_socket_fd, rd_buffer, 128) < 0){
        error_at_line(-1, errno, __FILE__, __LINE__, "Error from read()");
    }
    printf("Data received:\n %s", rd_buffer);

    close(client_socket_fd);

    return 0;
}
