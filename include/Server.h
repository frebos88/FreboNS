#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "DnsPacket.h"

#define BUFLEN 1024

class Server {
	public:
		Server();
		~Server();
		
		bool start(bool output);
		bool stop();
		bool getPacket();

	private:
		int sock;
		socklen_t clilen;
		struct sockaddr_in serv_addr, cli_addr;
		char buffer[BUFLEN];
		bool running;
		bool output;
		
		
		
		void error(const char *msg);

		DnsPacket dp;
};
