#include "DnsPacket.h"

#include <iostream>
#include <cstring>
#include <stdio.h>

DnsPacket::DnsPacket() {
	// Init
}

DnsPacket::~DnsPacket() {
	// Garbage
}

bool DnsPacket::parse(Packet p) {
	answer.clear();
	authority.clear();
	qname.clear();
	
	id = p.getShort();
	flags = p.getShort();
	flags = 0x8180;
	qdcount = p.getShort();
	ancount = p.getShort();
	ancount = 0;
	nscount = p.getShort();
	nscount = 0;
	arcount = p.getShort();
	arcount = 0;
	
	//printf("id: %u\n", id);
	/*printf("flags: %u\n", flags);
	printf("qdcount: %u\n", qdcount);
	printf("ancount: %u\n", ancount);
	printf("nscount: %u\n", nscount);
	printf("arcount: %u\n", arcount);*/
	
	char *tempString;
	while (tempString = p.getString()) {
		qname.push_back(::std::string(tempString));
	}
	
	if (!qname.size() > 1) return false;
	
	/*
	for (int i = 0; i < qname.size(); i++) {
		printf("String: %s\n", qname[i].c_str());
	}

	if( qname.size() == 3 )
	{
		std::cout << "Subdomain: " << qname[0].c_str() << std::endl;
		std::cout << "Domain: " << qname[1].c_str() << std::endl;
		std::cout << "TLD: " << qname[2].c_str() << std::endl;
	}
	else if( qname.size() == 2 )
	{
		std::cout << "Domain: " << qname[0].c_str() << std::endl;
		std::cout << "TLD: " << qname[1].c_str() << std::endl;
	}
	*/
	
	qtype = p.getShort();
	qclass = p.getShort();
	
	return true;
}

void DnsPacket::setId(short i) {
	id = i;
}

short DnsPacket::getId() {
	return id;
}

void DnsPacket::setFlags(short f) {
	flags = f;
}

short DnsPacket::getFlags() {
	return flags;
}

void DnsPacket::setQtype(unsigned short qt) {
	qtype = qt;
}

unsigned short DnsPacket::getQtype() {
	return qtype;
}

void DnsPacket::setQclass(unsigned short qc) {
	qclass = qc;
}

unsigned short DnsPacket::getQclass() {
	return qclass;
}

void DnsPacket::addAnswer(DnsAnswer* a) {
	answer.push_back(a);
	ancount++;
}

void DnsPacket::addAuthority(DnsAnswer* a) {
	authority.push_back(a);
	nscount++;
}

int DnsPacket::getRaw(char *buffer) {
	//char buffer[512];
	memset(buffer, 0, 512);
	Packet p(buffer);
	p.addShort(id);
	p.addShort(flags);//p.addShort(flags);
	p.addShort(qdcount);
	p.addShort(ancount);
	p.addShort(nscount);
	p.addShort(arcount);
	
	for (int i = 0; i < qname.size(); i++) {
		char *temp = (char*)qname[i].c_str();
		p.addString(temp);
	}
	p.addChar(0x00); // null terminator
	p.addShort(qtype);
	p.addShort(qclass);
	
	// Answer
	for (int y = 0; y < answer.size(); y++) {
		p.addShort(0xc00c); // pointer 0xc 0x00c
		Packet pa = answer[y]->getRaw();
		char *temp = pa.getRaw();
		for (int k = 0; k < pa.getSize(); k++)
			p.addChar(temp[k]);
	}
	
	// Authority
	for (int y = 0; y < authority.size(); y++) {
		p.addShort(0xc00c); // pointer 0xc 0x00c
		Packet pa = authority[y]->getRaw();
		char *temp = pa.getRaw();
		for (int k = 0; k < pa.getSize(); k++)
			p.addChar(temp[k]);
	}
	
	return p.getSize();
}

unsigned short DnsPacket::getQdcount() {
	return qdcount;
}

unsigned short DnsPacket::getAncount() {
	return ancount;
}

/*
void DnsPacket::addAuthority(DnsAuthority a) {
	
}*/

bool DnsPacket::isDomain()
{
	return ( qname.size() == 2 );
}

bool DnsPacket::isSubDomain()
{
	return ( qname.size() == 3 );
}

std::string DnsPacket::getSubDomain()
{
	if( isSubDomain() )
		return qname[0];

	return std::string( "" );
}

std::string DnsPacket::getDomain()
{
	std::string ret("");
	
	if( isSubDomain() )
	{
		ret.append( qname[1] );
		ret.append( "." );
		ret.append( qname[2] );
	}
	else
	{
		if (qname.size() == 1) {
			ret.append(qname[0]);
		} else if (qname.size() > 1) {
			ret.append( qname[0] );
			ret.append( "." );
			ret.append( qname[1] );
		}
	}

	return ret;
}

std::string DnsPacket::getQname( int index )
{
	if( qname.empty() || index < 0 || index > qname.size() )
		return std::string("");

	return qname[index];
}
