#pragma once

#include <vector>
#include <string>

#include "Packet.h"
#include "DnsAnswer.h"
//#include "DnsAuthority.h"

enum DNSType
{
	DNS_A = 1,
	DNS_NS = 2,
	DNS_SOA = 6,
};

class DnsPacket {
	public:
		DnsPacket();
		~DnsPacket();
	
		void setId(short i);
		short getId();
		
		void setFlags(short f);
		short getFlags();
		
		void setQtype(unsigned short qt);
		unsigned short getQtype();
		
		void setQclass(unsigned short qc);
		unsigned short getQclass();
		
		unsigned short getAncount();
	
		void addAnswer(DnsAnswer* a);
		void addAuthority(DnsAnswer* a);
		unsigned short getQdcount();
		
		int getRaw(char *buffer);

		bool isDomain();
		bool isSubDomain();

		std::string getSubDomain();
		std::string getDomain();
		std::string getQname( int index );
		
		bool parse(Packet p);
	
	private:
		// Header
		unsigned short id;
		unsigned short flags;
		unsigned short qdcount;
		unsigned short ancount;
		unsigned short nscount;
		unsigned short arcount;
		
		// Request
		::std::vector<std::string> qname;
		unsigned short qtype;
		unsigned short qclass;
		
		//Packet p;
		//char buffer[512];
		
		// Answers
		::std::vector<DnsAnswer*> answer;
		
		// Authority
		::std::vector<DnsAnswer*> authority;
};
