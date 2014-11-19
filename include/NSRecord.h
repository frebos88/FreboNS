#pragma once

#include <vector>
#include <string>
#include "DnsAnswer.h"

class NSRecord : public DnsAnswer {
	public:
		NSRecord(::std::string ns);
		NSRecord();
		~NSRecord();
		//virtual Packet getRaw();
		
		Packet getRaw();
		
		void addNs(::std::string ns);
		
		//void setIp(::std::string i);

	private:
		//::std::vector< std::vector<std::string> > nameservers;
		::std::vector<std::string> nameserver;
		
		unsigned short qtype;
		unsigned short qclass;
		unsigned int ttl;
		unsigned short rdLength;
		//int rData;
};