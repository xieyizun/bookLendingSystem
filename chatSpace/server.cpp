#include <iostream>
#include "Server.cpp"

int main() {
	Server *server = Server::getInstance();
	if (server) {
		server->startServer();
		server->ServerOn();
	}

	return 0;
}