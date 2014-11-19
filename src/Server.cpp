#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <time.h>

#include "Server.h"
#include "Packet.h"
#include "DnsPacket.h"
#include "ARecord.h"
#include "NSRecord.h"
#include "SOARecord.h"
#include "MXRecord.h"
#include "TXTRecord.h"
#include "CNAMERecord.h"
#include "PTRRecord.h"

#include "Database.h"

#define PORT 53

Server::Server() {
	running = false;
	output = false;
}

Server::~Server() {
	stop();
}

bool Server::start(bool output) {
	this->output = output;
	
	memset((char*)&serv_addr, 0, sizeof(serv_addr));
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);
	
	clilen = sizeof(cli_addr);
	
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}

	bool opt = true;
	setsockopt(sock, IPPROTO_IP, IP_PKTINFO, (char *)&opt, sizeof(opt));
	
	running = true;
}

bool Server::stop() {
	close(sock);
	running = false;
	
	return true;
}

bool Server::getPacket() {
	if (!running) return false;
	
	bzero(buffer, BUFLEN);
	struct iovec vector;
	vector.iov_base = buffer;
	vector.iov_len = sizeof(buffer);

	struct msghdr msg;
	msg.msg_name = &cli_addr;
	msg.msg_namelen = sizeof(cli_addr);
	msg.msg_iov = &vector;
	msg.msg_iovlen = 1;
	int flags = 0;

	char controlBuffer[1024];
	msg.msg_control = controlBuffer;
	msg.msg_controllen = 1024;

	// Recv packet
	int bytes = ::recvmsg(sock, &msg, flags);

	struct cmsghdr *cmsg;
	struct in_pktinfo *dest_ip_ptr;
	int dest_ip = 0;
	
	// Loop through IP header messages
	cmsg = CMSG_FIRSTHDR(&msg);
	for ( cmsg = CMSG_FIRSTHDR(&msg);
		cmsg != NULL;
		cmsg = CMSG_NXTHDR( &msg, cmsg ) )
	{
		if (cmsg->cmsg_level != IPPROTO_IP ||
			cmsg->cmsg_type != IP_PKTINFO)
		{
			continue;
		}
		
		// Get IP (int)
		struct in_pktinfo *dest_ip_ptr = (struct in_pktinfo*)CMSG_DATA(cmsg);
		dest_ip = dest_ip_ptr->ipi_addr.s_addr;
	}

	// Parse packet
	Packet p(buffer);
	bool dpParse = dp.parse(p);
	if (output) p.dump();

	// Format IP
	unsigned char ipParts[4];
	ipParts[0] = dest_ip & 0xFF;
	ipParts[1] = (dest_ip >> 8) & 0xFF;
	ipParts[2] = (dest_ip >> 16) & 0xFF;
	ipParts[3] = (dest_ip >> 24) & 0xFF;


	unsigned short qtype = (unsigned short)dp.getQtype();
	
	//"domains" table
	//+----+------------+----------------+-------------------+------------+---------+-------+--------+---------+
	//| id | name       | nameserver     | email             | serial     | refresh | retry | expire | minimum |
	//+----+------------+----------------+-------------------+------------+---------+-------+--------+---------+

	//"records" table
	//+----+-----------+------+------+------+----------------+----------+
	//| id | domain_id | type | name | ttl  | content        | priority |
	//+----+-----------+------+------+------+----------------+----------+

	if (dpParse) {
	
	MYSQL_RES* Result = NULL;
	
	clock_t tStart = clock();
	
	if (qtype != 6)
		 Result = Db->Query( Db->Format( "SELECT d.id, r.name, r.ttl, r.content, r.priority FROM domains d INNER JOIN records r ON r.domain_id = d.id WHERE d.name='%s' AND r.name='%s' AND r.type=%u", dp.getDomain().c_str(), dp.getSubDomain().c_str(), dp.getQtype() ) );
	else {
		if (!dp.isSubDomain())
			Result = Db->Query( Db->Format( "SELECT d.nameserver, d.email, d.serial, d.refresh, d.retry, d.expire, d.minimum FROM domains d WHERE d.name='%s'", dp.getDomain().c_str() ) );
	}
	
	if (Result) {
		MYSQL_ROW row;
			//dp.setFlags(0x8580); // authorative
			dp.setFlags(0x8400); // authorative, non-recursive

		while( row = Db->FetchRow( Result ) ) {
			if (qtype == 1) {
				dp.addAnswer(new ARecord(row[3]));
			} else if (qtype == 2) { // ns
				dp.addAnswer(new NSRecord(row[3]));
				//printf("Row: '%s' '%s' '%s' '%s'\n", row[0], row[1], row[2], row[3]);
			} else if (qtype == 5) { // CNAME
				dp.addAnswer(new CNAMERecord(row[3]));
			} else if (qtype == 12) { // PTR
				dp.addAnswer(new PTRRecord(row[3]));
			} else if (qtype == 16) { // TXT
				dp.addAnswer(new TXTRecord(row[3]));
			} else if (qtype == 15) { // mx
				dp.addAnswer(new MXRecord((unsigned short)atoi(row[4]), row[3]));
			} else if (qtype == 6) {
				dp.addAnswer(new SOARecord(row[0], row[1], atoi(row[2]), atoi(row[3]), atoi(row[4]), atoi(row[5]), atoi(row[6])));
			}
		}

		Db->Free( Result );
	
		//if (dp.getQdcount() > 0) {
		//if (qtype != 2 && qtype != 5 && dp.getQdcount() > 0 && dp.getAncount()) {
		if (qtype != 2 && dp.getQdcount() > 0 && dp.getAncount()) {
			dp.addAuthority(new NSRecord("ns1.frebos.com"));
			dp.addAuthority(new NSRecord("ns2.frebos.com"));
			/*dp.addAuthority(new NSRecord("ns3.frebos.com"));
			dp.addAuthority(new NSRecord("ns4.frebos.com"));
			dp.addAuthority(new NSRecord("ns5.frebos.com"));*/
		}
	}
	
	if (output) printf("Time taken for MySQL: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	
	} // if (dpParse) end
	
	// Build source sockaddr
	struct sockaddr_in src_addr;
    memset(&src_addr, 0, sizeof(struct sockaddr_in));
    src_addr.sin_family = AF_INET;
	
	char destIp[16];
	memset(destIp, 0, sizeof(destIp));
	sprintf(destIp, "%d.%d.%d.%d", ipParts[0], ipParts[1], ipParts[2], ipParts[3]);
    inet_aton(destIp, &(src_addr.sin_addr));
	
	char cmbuf[CMSG_SPACE(sizeof(struct in_pktinfo))];

	bzero(buffer, BUFLEN);
    int len = dp.getRaw(buffer);
	
	Packet pTemp(buffer);
	if (output) {
		pTemp.dump();
		printf("\n");
	}
	
    struct msghdr mh;
    memset(&mh, 0, sizeof(mh));

    struct cmsghdr *cmsg_send;
    struct in_pktinfo *pktinfo;

    struct iovec iov[1];
    iov[0].iov_base = buffer;
    //iov[0].iov_len = BUFLEN;
    iov[0].iov_len = len;

    mh.msg_name = &cli_addr; // destination address of packet
    mh.msg_namelen = sizeof(cli_addr);
    mh.msg_control = cmbuf;
    mh.msg_controllen = sizeof(cmbuf);
    mh.msg_flags = 0;
    mh.msg_iov = iov;
    mh.msg_iovlen = 1;

    // after initializing msghdr & control data to CMSG_SPACE(sizeof(struct in_pktinfo))
    cmsg_send = CMSG_FIRSTHDR(&mh);
    cmsg_send->cmsg_level = IPPROTO_IP;
    cmsg_send->cmsg_type = IP_PKTINFO;
    cmsg_send->cmsg_len = CMSG_LEN(sizeof(struct in_pktinfo));
    pktinfo = (struct in_pktinfo*) CMSG_DATA(cmsg_send);
    pktinfo->ipi_ifindex = 0;
    pktinfo->ipi_spec_dst = src_addr.sin_addr;

    int rc = sendmsg(sock, &mh, 0);
	
	return true;
}

void Server::error(const char *msg) {
        perror(msg);
        exit(1);
}
