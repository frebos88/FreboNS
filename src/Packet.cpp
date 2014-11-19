#include "Packet.h"

/**
 * TODO: Fix big/little endian support.
 */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Packet::Packet(char * raw) {
        this->raw = raw;
        this->pos = 0;
}

Packet::~Packet() {
	/* Clean up */
}

void Packet::addChar(char val) {
    raw[pos] = val;
    pos++;
}

char Packet::getChar() {
    char temp = raw[pos];
    pos++;
    return temp;
}

void Packet::addShort(unsigned short val) {
    /*raw[pos] = val & 0xff;
    raw[pos + 1] = (val >> 8)  & 0xff;*/
	raw[pos + 1] = val & 0xff;
    raw[pos] = (val >> 8)  & 0xff;
    pos += 2;
}

unsigned short Packet::getShort() {
    //short temp = (short)((raw[pos + 1]&0xff) << 8) | (raw[pos]&0xff);
	short temp = (short)((raw[pos]&0xff) << 8) | (raw[pos + 1]&0xff); // reversed
    pos += 2;
    return temp;
}

void Packet::addInt(int val) {
    /*raw[pos] = val & 0xff;
    raw[pos + 1] = (val >> 8)  & 0xff;
    raw[pos + 2] = (val >> 16) & 0xff;
    raw[pos + 3] = (val >> 24) & 0xff;
    */
	raw[pos + 3] = val & 0xff;
    raw[pos + 2] = (val >> 8)  & 0xff;
    raw[pos + 1] = (val >> 16) & 0xff;
    raw[pos] = (val >> 24) & 0xff;
	pos += 4;
}

int Packet::getInt() {
    //int temp = (int)((raw[pos + 3]&0xff) << 24) | ((raw[pos + 2]&0xff) << 16) | ((raw[pos + 1]&0xff) << 8) | (raw[pos]&0xff);
	int temp = (int)((raw[pos]&0xff) << 24) | ((raw[pos + 1]&0xff) << 16) | ((raw[pos + 2]&0xff) << 8) | (raw[pos + 3]&0xff);
    pos += 4;
    return temp;
}

char * Packet::getRaw() {
    return raw;
}

int Packet::getSize() {
    return pos;
}

void Packet::addString(char *str) {
    int sLen = strlen(str);
    addChar(sLen);
    strncpy((char*)raw+pos, str, sLen);
    pos += sLen;
}

char* Packet::getString() {
        int strlen = (int)getChar();
        if (!strlen) return 0;
        char *temp = (char*)malloc(strlen + 1);
        memset(temp, 0, (strlen + 1));
        strncpy(temp, &raw[pos], strlen);

        std::string temp2(temp);
        free(temp);

        this->pos += strlen;
        return (char*)temp2.c_str();
}

void Packet::dump() {
	// Print timestamp first
	time_t now = time(0);
	struct tm tstruct;
	char timeBuf[12];
	tstruct = *localtime(&now);
	strftime(timeBuf, sizeof(timeBuf), "%X", &tstruct);
	
	// Print data
	int y = 0;
	printf("%s - Raw data: ", timeBuf);
	for (int i = 0; i < 80; i++) {
		if (y < 1) {
			printf(" %02x",(unsigned char)raw[i]); // 01e20e format
			y++;
		} else {
			printf("%02x",(unsigned char)raw[i]); // 01e20e format
			y = 0;
		}
	}
	printf("\n");
}
