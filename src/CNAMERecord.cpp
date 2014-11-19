#include "CNAMERecord.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>

CNAMERecord::CNAMERecord() {
	qtype = 5;
	qclass = 1;
	ttl = 600;
	rdLength = 1; // ending null terminator byte
}

CNAMERecord::CNAMERecord(::std::string str) {
	qtype = 5;
	qclass = 1;
	ttl = 600;
	rdLength = 1; // ending null terminator byte
	
	::std::istringstream iss(str);
	::std::string match;
	while (::std::getline(iss, match, '.')) {
		cname.push_back(match);
		rdLength += match.length() + 1;
	}
}

CNAMERecord::~CNAMERecord() {
	// Garbage
}


void CNAMERecord::setCName(::std::string str) {
	::std::istringstream iss(str);
	::std::string match;
	
	cname.empty();
	rdLength = 1;
	
	while (::std::getline(iss, match, '.')) {
		cname.push_back(match);
		rdLength += match.length() + 1;
	}
	
	return;
}


Packet CNAMERecord::getRaw() {
	char buffer[512];
	memset(buffer, 0, 512);
	Packet p(buffer);
	p.addShort(qtype);
	p.addShort(qclass);
	p.addInt(ttl);
	p.addShort(rdLength);
	
	// rData
	for (int i = 0; i < cname.size(); i++)
			p.addString((char*)cname[i].c_str());
	p.addChar(0);
	
	return p;
}