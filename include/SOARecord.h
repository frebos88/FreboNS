#pragma once

#include <vector>
#include <string>
#include "DnsAnswer.h"

class SOARecord : public DnsAnswer {
	public:
		//SOARecord();
		SOARecord(::std::string ns, ::std::string e, int serial, int refresh, int retry, int expire, int minimum);
		~SOARecord();
		//virtual Packet getRaw();
		
		Packet getRaw();
		
		//void setIp(::std::string i);

	private:
		unsigned short qtype;
		unsigned short qclass;
		unsigned int ttl;
		unsigned short rdLength;
		
		// rData
		::std::vector<std::string> nameserver;
		::std::vector<std::string> email;
		int serial;
		int refresh;
		int retry;
		int expire;
		int minimum;
};