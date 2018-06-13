#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>  // htons/htonl
#include <unistd.h>     // read/write/close
#include <iostream>
#include <functional>
#include <thread>

using namespace std::placeholders;  // for _1, _2, ...

void runClient(const char *srvAddr, const uint16_t port)
{
    printf("run clinet\n");
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("create socket error\n");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    //addr.sin_addr.s_addr = htonl(srvAddr);
    inet_pton(AF_INET, srvAddr, &addr.sin_addr);
    addr.sin_port = htons(port);
    int res = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        printf("connect error, remote server not found\n");
    }

    char buf[256];
    do {
        std::cin >> buf;
        int nw = write(fd, buf, sizeof(buf));
        if (nw != sizeof(buf))
            printf("nw != sizeof(buf)\n");
    } while (strcmp(buf, "q") == 0);

    shutdown(fd, SHUT_WR);  // �ر�д

    int nr = 0;
    while ((nr = read(fd, buf, sizeof(buf))) > 0) {
        
    }
    close(fd);
}

void runServer(const uint16_t port)
{
    printf("run server\n");

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("create socket error\n");
    }

    struct sockaddr_in srvAddr;
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(port);
    int res = bind(fd, (struct sockaddr*)&srvAddr, sizeof(srvAddr));
    if (res == -1) {
        printf("bind error\n");
    }

    res = listen(fd, 10);
    if (res == -1) {
        printf("listen error\n");
    }

    struct sockaddr connAddr;
    socklen_t addrlen = sizeof(connAddr);   // socklen_t ������
    int connfd = -1;
    int nr = 0;
    char buf[256];
    while (1) {
        connfd = accept(fd, (struct sockaddr*)&connAddr, &addrlen);
        //printf("accept address: %s, connfd: %d\n", inet_ntoa(connAddr.sin_addr), connfd);
        printf("accept connfd: %d\n", connfd);
        if (connfd != -1) {
            while ((nr = read(connfd, buf, sizeof(buf))) > 0) {
                buf[nr] = '\0';
                printf("recv: %s\n", buf);
                write(connfd, buf, sizeof(buf));
            }
            close(connfd);
            printf("server close connfd: %d\n", connfd);
        }
    }
}

class TcpClient
{
public:
    TcpClient(std::string srvAddr, uint16_t port)
    {
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ == -1)
            printf("create socket error\n");

        addr_.sin_family = AF_INET;
        inet_pton(AF_INET, srvAddr.c_str(), &addr_.sin_addr);
        addr_.sin_port = htons(port);

        this->connectCb(std::bind(TcpClient::onConnectCallback));
        this->messageCb(std::bind(TcpClient::onMessageCallback, _1, _2));
    }
    ~TcpClient()
    {
        ::close(fd_);
    }

    bool connect()
    {
        int res = ::connect(fd_, (struct sockaddr*)&addr_, sizeof(addr_));
        if (res == -1)
            return false;

        connectCb_();

        int nr = 0;
        char buf[1024];
        while (1) {
            while ((nr = read(fd_, buf, sizeof(buf))) > 0) {
                // �ֶ�ζ�ȡ
                messageCb_(std::string(buf), sizeof(buf));
            }
        }
        return true;
    }

    void send(const std::string buf, int bufsize)
    {
        // buf ̫��ֶ�η���
        write(fd_, buf.c_str(), buf.size());
    }

    static void onConnectCallback()
    {
        printf("onConnectCallback\n");
    }

    static void onMessageCallback(std::string buf, int len)    // message input
    {
        //printf("onMessagecCallback: %s\n", buf.c_str());
        std::cout << "onMessageCallback:" + buf << std::endl;
    }
    
private:
    typedef std::function<void(std::string, int)> MessageCbFunc;
    TcpClient & operator=(TcpClient & rhs) {}

    void connectCb(std::function<void()> cb) { connectCb_ = cb; }
    void messageCb(MessageCbFunc cb) { messageCb_ = cb; }

    std::function<void()> connectCb_;
    MessageCbFunc messageCb_;
    struct sockaddr_in addr_;
    int fd_;
};

class TcpServer
{
public:
    TcpServer(uint16_t port)
        : port_(port)
    {
        printf("TcpServer\n");
        fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ == -1) {
            printf("create socket error\n");
        }

        addr_.sin_family = AF_INET;
        addr_.sin_addr.s_addr = htonl(INADDR_ANY);
        addr_.sin_port = htons(port);
        if (bind(fd_, (struct sockaddr*)&addr_, sizeof(addr_)) == -1) {
            printf("bind error\n");
        }

        connectCb(std::bind(TcpServer::onConnectionCallback, _1));
        messageCb(std::bind(TcpServer::onMessageCallback, _1, _2, _3));
    }
    ~TcpServer()
    {
        printf("TcpServer\n");
        close(fd_);
    }

    bool start()
    {
        printf("start\n");
        if (listen(fd_, 10) == -1) {
            printf("listen error\n");
        }

        struct sockaddr connAddr;
        socklen_t addrlen = sizeof(connAddr);   // socklen_t ������
        int connfd = -1;
        int nr = 0;
        char buf[256];
        while (1) {
            connfd = accept(fd_, (struct sockaddr*)&connAddr, &addrlen);
            //printf("accept address: %s, connfd: %d\n", inet_ntoa(connAddr.sin_addr), connfd);
            connectCb_(connfd);

            std::thread([this](MessageCbFunc messageCb, int fd) {
                printf("thread start...\n");
                int nr = 0;
                char buf[256];
                while (1) {
                    if (fd != -1) {
                        while ((nr = read(fd, buf, sizeof(buf))) > 0) {
                            buf[nr] = '\0';
                            //printf("recv: %s\n", buf);
                            messageCb(fd, std::string(buf), sizeof(buf));
                            //write(connfd, buf, sizeof(buf));
                        }
                        //close(connfd);
                        //printf("server close connfd: %d\n", connfd);
                    }
                }
            }, messageCb_, connfd);
        }
    }

    static void onConnectionCallback(int connfd)
    {
        printf("onConnectionCallback connfd: %d\n", connfd);
        //std::thread([](connfd) {
        //    while (1) {
        //        int nr = 0;
        //        char buf[256];
        //        while ((nr = read(connfd, buf, sizeof(buf))) > 0) {
        //            buf[nr] = '\0';
        //            printf("recv: %s\n", buf);
        //            //write(connfd, buf, sizeof(buf));
        //            //messageCb_();
        //        }
        //    }
        //});
    }

    static void onMessageCallback(int connfd, std::string buf, int len)
    {
        //printf("onMessageCallback: %s\n", buf.c_str());
        std::cout << "onMessageCallback:" + buf << std::endl;
    }

private:
    typedef std::function<void(int, std::string, int)> MessageCbFunc;
    void connectCb(std::function<void(int)> cb) { connectCb_ = cb; }
    void messageCb(MessageCbFunc cb) { messageCb_ = cb; }

    int fd_;
    struct sockaddr_in addr_;
    uint16_t port_;
    std::function<void(int)> connectCb_;
    MessageCbFunc messageCb_;
};

int main(int argc, char *argv[])
{
    if (argc == 4) {
        if (strcmp(argv[1], "-c") == 0) {
            //runClient(argv[2], static_cast<uint16_t>(atoi(argv[3])));
            TcpClient client(argv[2], static_cast<uint16_t>(atoi(argv[3])));
            client.connect();
            std::string data;
            std::cin >> data;
            while (data != "q") {
                std::cin >> data;
                client.send(data, data.length());
            }
        }
    }
    else if (argc == 3) {
        if (strcmp(argv[1], "-l") == 0) {
            //runServer(static_cast<uint16_t>(atoi(argv[2])));
            TcpServer server(static_cast<uint16_t>(atoi(argv[2])));
            server.start();
        }
    }
    else {
        printf("Client Usage: -c server_address port\n");
        printf("Server Usage: -l port\n");
    }

    // 0. ����������װ����
    // 1. �ͻ��˳�������3���̣߳�connect����ͬ��3������ˣ�Ϊÿ̨�����������
    // 2. ÿ���̴߳�ͬһ�ļ������ʣ�����hashֵ���͵���ͬ��3̨������
    // 3. ���������յ����ݺ�����д���ļ�
    // 4. merge�������ӷ����������ݣ�ȡ��topK����
}



/*
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
    inet_pton(AF_INET, serverAddr, &addr.sin_addr);  // pָ�� to network address
    addr.sin_port = htons(port); // converts the unsigned short integer hostshort from host byte order to network byte order.

    // 1. ��λ�֪Զ�̷�����������
    // 2. ��β�����������
    int res = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (res == -1) {
        printf("connect error, remote server not found\n");
        exit(1);
    }

    // 3. write/read
    char data[256];
    do {
        std::cin >> data;
        // 3. �������ʱ���������ѹر���ô��
        int writeLen = write(fd, data, sizeof(data));
    } while (strcmp(data, "q") != 0);

    // �ͻ��˹ر���ʲôӰ�죿
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
        // ��λ�֪�ͻ��˶Ͽ�����
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

*/