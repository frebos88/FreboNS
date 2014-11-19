#include "NSRecord.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>

NSRecord::NSRecord() {
	qtype = 2;
	qclass = 1;
	ttl = 600;
	rdLength = 1; // ending null terminator byte
}

NSRecord::NSRecord(::std::string ns) {
	qtype = 2;
	qclass = 1;
	ttl = 600;
	rdLength = 1; // ending null terminator byte
	
	::std::istringstream iss(ns);
	::std::string match;
	while (::std::getline(iss, match, '.')) {
		nameserver.push_back(match);
		rdLength += match.length() + 1;
	}
}

NSRecord::~NSRecord() {
	// Garbage
}

/*void NSRecord::setIp(::std::string i) {
	int temp = inet_addr(i.c_str());
	rData = __builtin_bswap32(temp);
	rdLength += 4;
}*/
/*
void NSRecord::addNs(::std::string ns) {
	::std::vector<std::string> temp;
	
	::std::istringstream iss(ns);
	::std::string match;
	while (::std::getline(iss, match, '.')) {
		temp.push_back(match);
		rdLength += match.length() + 1;
	}
	
	nameservers.push_back(temp);
	return;
}*/


Packet NSRecord::getRaw() {
	char buffer[512];
	memset(buffer, 0, 512);
	Packet p(buffer);
	p.addShort(qtype);
	p.addShort(qclass);
	p.addInt(ttl);
	p.addShort(rdLength);
	
	// rData
	for (int i = 0; i < nameserver.size(); i++)
			p.addString((char*)nameserver[i].c_str());
	p.addChar(0);
	
	return p;
}