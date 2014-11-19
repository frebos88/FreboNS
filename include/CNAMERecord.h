#pragma once

#include <vector>
#include <string>
#include "DnsAnswer.h"

class CNAMERecord : public DnsAnswer {
	public:
		CNAMERecord(::std::string str);
		CNAMERecord();
		~CNAMERecord();
		//virtual Packet getRaw();
		
		Packet getRaw();
		
		void setCName(::std::string str);

	private:
		::std::vector<std::string> cname;
		
		unsigned short qtype;
		unsigned short qclass;
		unsigned int ttl;
		unsigned short rdLength;
};