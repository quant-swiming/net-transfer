#pragma once

#include<unordered_map>
class TcpServer {
	public:
		int init(const char *ip, int port);
		void run();
		virtual void handler(int fd) = 0;
	private:
		char serverIp_ [32];
		int serverPort_;
		int serverSockFd_;
		int clientSockFd_; 
		int maxId;
};
