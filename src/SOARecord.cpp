#include "SOARecord.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <arpa/inet.h>

/*SOARecord::SOARecord() {
	qtype = 6;
	qclass = 1;
	ttl = 600;
	rdLength = 2;
}*/

SOARecord::SOARecord(::std::string ns, ::std::string e, int serial, int refresh, int retry, int expire, int minimum) {
	qtype = 6;
	qclass = 1;
	ttl = 3600;
	rdLength = 22; // 2 null terminators, 5 ints
	
	::std::istringstream iss(ns);
	::std::string match;
	while (::std::getline(iss, match, '.')) {
		nameserver.push_back(match);
		rdLength += match.length() + 1;
	}
	
	::std::istringstream iss2(e);
	::std::string match2;
	while (::std::getline(iss2, match2, '.')) {
		email.push_back(match2);
		rdLength += match2.length() + 1;
	}
	
	this->serial = serial;
	this->refresh = refresh;
	this->retry = retry;
	this->expire = expire;
	this->minimum = minimum;
}

SOARecord::~SOARecord() {
	// Garbage
}

/*void SOARecord::setIp(::std::string i) {
	int temp = inet_addr(i.c_str());
	rData = __builtin_bswap32(temp);
	rdLength += 4;
	return;
}*/

Packet SOARecord::getRaw() {
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
	
	for (int i = 0; i < email.size(); i++)
			p.addString((char*)email[i].c_str());
	p.addChar(0);
	
	p.addInt(serial);
	p.addInt(refresh);
	p.addInt(retry);
	p.addInt(expire);
	p.addInt(minimum);
	
	return p;
}