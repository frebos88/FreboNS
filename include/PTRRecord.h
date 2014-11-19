#pragma once

#include <string>
#include "DnsAnswer.h"

class PTRRecord : public DnsAnswer {
	public:
		PTRRecord();
		PTRRecord(::std::string i);
		~PTRRecord();
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