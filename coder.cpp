#include <iostream>
#include <fstream>

#include "coder.h"

unsigned int coder::splitrange( float prediction ) {
    unsigned int range = (unsigned int)((high - low) * prediction) + low;
    if ( high <= range ) range = high - 1;
    if ( low >= range ) range = low + 1;
    return range;
}

void coder::shiftup() {
    low = (low & 0x7FFFFFFF) << 1;
    high = ((high & 0x7FFFFFFF) << 1) | 1;
}

unsigned int coder::inputbit() {
    static unsigned int bitpos = 8;
    static unsigned int byte;
    if ( bitpos == 8 ) {
        if ( !std::cin.read( (char*)&byte, 1 ) ) return EOF;
        bitpos = 0;
    }
    ++bitpos;
    byte = byte << 1;
    return (byte & 0x100) >> 8;
}

void coder::outputbit( unsigned int bit ) {
    static unsigned int bitpos = 0;
    static unsigned int byte = 0;
    ++bitpos;
    byte = (byte << 1) | bit;
    if ( bitpos == 8 ) {
        std::cout.write( (char*)&byte, 1 );
        bitpos = 0;
        byte = 0;
    }
}

coder::coder() {
    low = 0x0;
    high = 0xFFFFFFFF;
}

unsigned long long coder::encode( mixer p ) {
    unsigned long long bitsread = 0;
    unsigned long long bitswritten = 0;
    for ( int bit = inputbit(); bit != EOF; bit = inputbit() ) {
        unsigned int cur = splitrange( p.get_prediction() );
        bit ? low = cur + 1 : high = cur;
        p.model( bit );
        while ( ((~high) ^ low) & 0x80000000 ) {
            outputbit( (high & 0x80000000) >> 31 );
            shiftup();
            ++bitswritten;
        }
        if ( bitsread % 101020 == 0 ) std::cerr << "\r" << bitsread/8 << " bytes read and " << bitswritten/8 << " bytes written   ratio = " << ((float)(bitswritten+1))/((float)(bitsread+1));
        ++bitsread;
    }

    for ( int i=0; i<32; ++i ) {
        outputbit( (high & 0x80000000) >> 31 );
        shiftup();
    }

    std::cerr << "\r" << bitsread/8 << " bytes read and " << bitswritten/8 << " bytes written   ratio = " << ((float)(bitswritten+1))/((float)(bitsread+1)) << std::endl;
    return bitsread/8;
}

void coder::decode( mixer p, unsigned long long filesize ) {
    unsigned int filebits = 0;
    for ( int i = 0; i < 32; ++i ) {
        unsigned int input = inputbit();
        input != (unsigned int)EOF ? filebits = ((filebits & 0x7FFFFFFFul) << 1ul) | input : filebits = ((filebits & 0x7FFFFFFFul) << 1ul);
    }
    unsigned long long bitswritten = 0;
    unsigned long long bitsread = 32;
    while ( bitswritten/8 < filesize ) {
        unsigned int cur = splitrange( p.get_prediction() );
        unsigned int bit;
        filebits <= cur ? bit = 0 : bit = 1;
        outputbit( bit );
        bit ? low = cur + 1 : high = cur;
        p.model( bit );
        while ( ((~high) ^ low) & 0x80000000 ) {
            unsigned int input = inputbit();
            input != (unsigned int)EOF ? filebits = ((filebits & 0x7FFFFFFF) << 1) | input : filebits = ((filebits & 0x7FFFFFFF) << 1);
            shiftup();
            ++bitsread;
        }
        if ( bitswritten % 100113 == 0 ) std::cerr << "\r" << bitsread/8 << " bytes read and " << bitswritten/8 << " bytes written  ratio = " << (float)bitswritten/(float)bitsread;
        ++bitswritten;
    }
    std::cerr << "\r" << bitsread/8 << " bytes read and " << bitswritten/8 << " bytes written" << std::endl;
}
