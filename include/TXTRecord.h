#pragma once

#include <vector>
#include <string>
#include "DnsAnswer.h"

class TXTRecord : public DnsAnswer {
	public:
		TXTRecord(::std::string str);
		TXTRecord();
		~TXTRecord();
		//virtual Packet getRaw();
		
		Packet getRaw();
		
		void addString(::std::string str);

	private:
		::std::vector<std::string> strings;
		
		unsigned short qtype;
		unsigned short qclass;
		unsigned int ttl;
		unsigned short rdLength;
};