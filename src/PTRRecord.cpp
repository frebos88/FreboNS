#include "PTRRecord.h"

#include <cstring>
#include <arpa/inet.h>

PTRRecord::PTRRecord() {
	qtype = 12;
	qclass = 1;
	ttl = 600;
	rdLength = 0;
}

PTRRecord::PTRRecord(::std::string i) {
	qtype = 12;
	qclass = 1;
	ttl = 600;
	rdLength = 0;
	setIp(i);
}

PTRRecord::~PTRRecord() {
	// Garbage
}

void PTRRecord::setIp(::std::string i) {
	int temp = inet_addr(i.c_str());
	rData = __builtin_bswap32(temp);
	rdLength += 4;
	return;
}

Packet PTRRecord::getRaw() {
	char buffer[512];
	memset(buffer, 0, 512);
	Packet p(buffer);
	p.addShort(qtype);
	p.addShort(qclass);
	p.addInt(ttl);
	p.addShort(rdLength);
	p.addInt(rData);
	return p;
}