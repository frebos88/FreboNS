#pragma once

class Packet
{
    public:
        Packet(unsigned int length);
        Packet(char * raw);
        ///header

        void addChar(char val);
        char getChar();

        void addShort(unsigned short val);
        unsigned short getShort();

        void addInt(int val);
        int getInt();

        void addLong(long long val);
        long getLong();

        void addString(char * str);
        char * getString();

        char * getRaw();
        int getSize();
		
		void dump();

        ~Packet();
    protected:
    private:
        char * raw;
        unsigned int pos;
};
