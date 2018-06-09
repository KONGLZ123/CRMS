#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

void runClient(char *serverAddr, int port)
{
    printf("client run\n");
    // 1. socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("create socket failed");
    }

    // 2. connect
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    //clientAddr.sin_addr.s_addr = htonl(serverAddr);
    inet_pton(AF_INET, serverAddr, &addr.sin_addr);  // p指针 to network address
    addr.sin_port = htons(port); // converts the unsigned short integer hostshort from host byte order to network byte order.

    // 1. 如何获知远程服务器不存在
    // 2. 如何不断重试重连
    int res = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        printf("connect error, remote server not found\n");
        exit(1);
    }

    // 3. write/read
    char data[256];
    do {
        std::cin >> data;
        // 3. 如果发送时，服务器已关闭怎么办
        int writeLen = write(fd, data, sizeof(data));
    } while (strcmp(data, "q") != 0);

    // 客户端关闭有什么影响？
    close(fd);
    printf("client close\n");
}

void runServer(int port)
{
    printf("server run\n");
    // 1. socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("create socket error\n");
    }

    // 2. bind
    int res = 0;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    res = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        printf("bind error\n");
    }

    // 3. listen
    res = listen(fd, 10);
    if (res == -1) {
        printf("listen error\n");
    }

    // 4. accept
    char buf[256];
    socklen_t addrlen = sizeof(addr);
    ssize_t readNum = 0;
    int q = 1;
    while (q) {
        // 如何获知客户端断开连接
        int connfd = accept(fd, (struct sockaddr*)&addr, &addrlen);
        if (connfd != -1) {
            printf("accept\n");
            // 5. write/read
            while ((readNum = read(connfd, buf, 256)) > 0) {
                printf("receive: %s\n", buf);
                buf[readNum] = '\n';
                write(connfd, buf, sizeof(buf));
            }
            
            if (strcmp(buf, "shutdown") == 0) {
                q = 0;
                close(connfd);
                printf("server close \n");
            }
        }
        else {
            printf("client close\n");
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc == 4) {
        if (!strcmp("-c", argv[1])) {
            runClient(argv[2], atoi(argv[3]));
        }
    }
    else if (argc == 3) {
        if (!strcmp("-l", argv[1])) {
            runServer(atoi(argv[2]));
        }
    }
    else {
        printf("Client Usage: -c server_address port\n");
        printf("Server Usage: -l port\n");
    }

    return 0;
}