#ifndef CODER_H_INCLUDED
#define CODER_H_INCLUDED

#include "mixer.h"

/*
* An arithmetic coder
*/
class coder {
	unsigned int low;
	unsigned int high;

	unsigned int splitrange( float prediction );

	void shiftup();

	unsigned int inputbit();

	void outputbit( unsigned int bit );
public:

	coder();

	unsigned long long encode( mixer p );

	void decode( mixer p, unsigned long long filesize );
};

#endif // CODER_H_INCLUDED
