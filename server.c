#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


int create_socket()
{
    int socket_fd;
    struct sockaddr_in addr;

    int port = 9000;
    int backlog = 5;

    socket_fd = socket(AF_INET, SOCK_STREAM /*| SOCK_NONBLOCK*/, 0); //non-blocking socket
    if (socket_fd == -1) {
        error_at_line(-1, errno, __FILE__, __LINE__, "Error from socket()");
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
//    addr.sin_addr.s_addr = htonl(INADDR_ANY); //which addr? localhost?
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(socket_fd, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)) == -1){
        error_at_line(-1, errno, __FILE__, __LINE__, "Error from bind()");
    }

    if (listen(socket_fd, backlog) == -1){
        error_at_line(-1, errno, __FILE__, __LINE__, "Error from listen()");
    }

    return socket_fd;
}


int main()
{
    int server_sockfd;
    int new_sockfd;
    char wr_buffer[128];
    int len;

    server_sockfd = create_socket();

    new_sockfd =  accept(server_sockfd, 0, 0);
    if (new_sockfd == -1){
      error_at_line(-1, errno, __FILE__, __LINE__, "Error from accept()");
    }
    strcpy(wr_buffer, "Djes' clijent?\n");
    len = 16;
    if(write(new_sockfd, wr_buffer, len) < 0){
        error_at_line(-1, errno, __FILE__, __LINE__, "Error from write()");
    }

    close(new_sockfd);
    close(server_sockfd);

    return 0;
}
