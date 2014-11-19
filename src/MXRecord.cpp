#include "MXRecord.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <arpa/inet.h>

MXRecord::MXRecord() {
	qtype = 15;
	qclass = 1;
	ttl = 3600;
	rdLength = 1; // host null terminator
}

MXRecord::MXRecord(unsigned short priority, ::std::string host) {
	qtype = 15;
	qclass = 1;
	ttl = 3600;
	rdLength = 1;
	this->priority = priority;
	rdLength += sizeof(priority);
	setHost(host);
}

MXRecord::~MXRecord() {
	// Garbage
}

void MXRecord::setHost(::std::string host) {
	::std::istringstream iss(host);
	::std::string match;
	while (::std::getline(iss, match, '.')) {
		this->host.push_back(match);
		rdLength += match.length() + 1;
	}
	
	return;
}

void MXRecord::setPriority(unsigned short priority) {
	this->priority = priority;
	return;
}

Packet MXRecord::getRaw() {
	char buffer[512];
	memset(buffer, 0, 512);
	Packet p(buffer);
	p.addShort(qtype);
	p.addShort(qclass);
	p.addInt(ttl);
	p.addShort(rdLength);
	p.addShort(priority);
	
	// rData
	for (int i = 0; i < host.size(); i++)
			p.addString((char*)host[i].c_str());
	p.addChar(0);
	
	return p;
}