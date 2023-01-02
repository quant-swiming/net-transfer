#pragma once
#include "./tcpServer.h"


class TranServer:public TcpServer {
	public:
	private:
		void handler(int fd);
};
