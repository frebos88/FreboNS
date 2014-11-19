#pragma once

#include <vector>
#include <string>
#include "DnsAnswer.h"

class MXRecord : public DnsAnswer {
	public:
		MXRecord();
		MXRecord(unsigned short priority, ::std::string host);
		~MXRecord();
		//virtual Packet getRaw();
		
		Packet getRaw();
		
		void setHost(::std::string host);
		void setPriority(unsigned short priority);

	private:
		unsigned short qtype;
		unsigned short qclass;
		unsigned int ttl;
		unsigned short rdLength;
		
		// rData
		unsigned short priority;
		::std::vector<std::string> host;
};