#include "../include/tranServer.h"

int main() {
	TranServer server;
	server.init("127.0.0.1", 9091);
	server.run();
}
