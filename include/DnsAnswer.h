#pragma once

#include "Packet.h"

class DnsAnswer {
	public:
		DnsAnswer() {};
		virtual ~DnsAnswer() {};
		virtual Packet getRaw() = 0;
};