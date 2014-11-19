#include "TXTRecord.h"

#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>

TXTRecord::TXTRecord() {
	qtype = 16;
	qclass = 1;
	ttl = 600;
	rdLength = 0;
}

TXTRecord::TXTRecord(::std::string str) {
	qtype = 16;
	qclass = 1;
	ttl = 600;
	rdLength = 0;

	strings.clear();
	
	strings.push_back(str);
	rdLength += str.length() + 1;
}

TXTRecord::~TXTRecord() {
	// Garbage
}


void TXTRecord::addString(::std::string str) {
	strings.push_back(str);
	rdLength += str.length() + 1;
	
	return;
}


Packet TXTRecord::getRaw() {
	char buffer[512];
	memset(buffer, 0, 512);
	Packet p(buffer);
	p.addShort(qtype);
	p.addShort(qclass);
	p.addInt(ttl);
	p.addShort(rdLength);
	
	// rData
	for (int i = 0; i < strings.size(); i++)
			p.addString((char*)strings[i].c_str());
	
	return p;
}
