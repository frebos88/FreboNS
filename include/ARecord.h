#pragma once

#include <string>
#include "DnsAnswer.h"

class ARecord : public DnsAnswer {
	public:
		ARecord();
		ARecord(::std::string i);
		~ARecord();
		//virtual Packet getRaw();
		
		Packet getRaw();
		
		void setIp(::std::string i);

	private:
		unsigned short qtype;
		unsigned short qclass;
		unsigned int ttl;
		unsigned short rdLength;
		int rData;
};