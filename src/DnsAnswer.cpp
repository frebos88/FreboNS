#include "DnsAnswer.h"

Packet DnsAnswer::getRaw() {
	char buffer[1];
	return Packet(buffer);
}