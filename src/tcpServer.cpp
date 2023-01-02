#include "../include/tcpServer.h"
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<signal.h>
#include<sys/epoll.h>
#include<unistd.h>
void signalHandler(int s) {

}

int TcpServer::init(const char *ip, int port) {	
	printf("init: ip:%s port:%d\n", ip, port);
	strncpy(serverIp_, ip, sizeof(serverIp_));
        serverPort_ = port; 
	serverSockFd_ = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSockFd_ == -1) { 
		printf("init server socket fail\n");
		return -1;
	}
	int flag = 1;
	if(setsockopt(serverSockFd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
		printf("set reuseraddr flag error\n");
		return -1;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);
	if(bind(serverSockFd_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		printf("bind socket fail\n");
		return -1;
	}

	if(listen(serverSockFd_, 1024) == -1) {
		printf("listen fail\n");
		return -1;
	}
	signal(SIGPIPE, signalHandler);
        clientSockFd_ = 0; 
        maxId = 0;
	return 0;
}

void TcpServer::run() {
	int efd = epoll_create(1024);
	if(efd < 0 ) {
		printf("init epoll fd fail\n");
		return;
	}
	struct epoll_event event;
	event.events = EPOLLIN;
	if(epoll_ctl(efd, EPOLL_CTL_ADD, serverSockFd_, &event) < 0) {
		printf("epool ctl fail\n");
		return;
	}
	struct epoll_event resEvent;
	while(1) {
		int n = epoll_wait(efd, &resEvent, 1, 0);
		if(n == -1) {
			printf("epoll wait error\n");
			return;
		}
		if(n == 0) {
			continue;
		}
		if(resEvent.events & EPOLLIN) {
			printf("new connect\n");
			int fd = accept(serverSockFd_, NULL, NULL);
			int son_pid = fork();
			if(son_pid == -1) {
				printf("fork new procees fail\n");
				return;
			} else if(son_pid == 0) {
				printf("son\n");
				close(serverSockFd_);
				handler(fd);
				exit(0);
			} else {
				printf("new process %d\n", son_pid);
				close(fd);
			}
		}
	}

}

