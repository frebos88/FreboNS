#include "ARecord.h"

#include <cstring>
#include <arpa/inet.h>

ARecord::ARecord() {
	qtype = 1;
	qclass = 1;
	ttl = 600;
	rdLength = 0;
}

ARecord::ARecord(::std::string i) {
	qtype = 1;
	qclass = 1;
	ttl = 600;
	rdLength = 0;
	setIp(i);
}

ARecord::~ARecord() {
	// Garbage
}

void ARecord::setIp(::std::string i) {
	int temp = inet_addr(i.c_str());
	rData = __builtin_bswap32(temp);
	rdLength += 4;
	return;
}

Packet ARecord::getRaw() {
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
