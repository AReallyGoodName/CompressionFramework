#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "fileheader.h"
#include "mixer.h"
#include "coder.h"

using namespace std;

const unsigned int MAGIC_NUMBER = 0x62616C67; // File format specifier (GDMC)
const unsigned int VERSION_NUMBER = 1;

int main( int argc, char *argv[] ) {
	bool do_compression;
	bool got_compression_opt = false;
	fileheader description = { 0x0, 0x1000000, 0x0, 0x0 };

	for ( int i = 1; i < argc; ++i ) {
		if ( !strncmp( argv[i], "-c", 2 ) ) {
			do_compression = true;
			got_compression_opt = true;
		}
		else if ( !strncmp( argv[i], "-e", 2 ) ) {
			do_compression = false;
			got_compression_opt = true;
		}
		else if ( !strncmp( argv[i], "-m=", 3 ) ) {
			description.memory = atoll( &argv[i][3] ) * 1024l * 1024l; // In megabytes
		}
	}

	if ( do_compression && got_compression_opt ) {
		cout.write( (char*)&description, sizeof( fileheader ) );
		if ( description.memory == 0 ) {
            description.memory = 512l * 1024l * 1024l;
		}
		cerr << "Compressing with " << description.memory << " bytes of memory" << endl;
		mixer p(description.memory);
		coder c;
		cout.clear();
		description.decodedsize = c.encode( p );
		description.magicnumber = MAGIC_NUMBER;
		description.version = VERSION_NUMBER;
		rewind( stdout );
		cout.write( (char*)&description, sizeof( fileheader ) );
		return 0;
	}
	else if ( !do_compression && got_compression_opt ) {
		cin.read( (char*)&description, sizeof( fileheader ) );
		if ( description.magicnumber != MAGIC_NUMBER || description.version != VERSION_NUMBER ) {
			cerr << "Not a gdmcV1 file" << description.reserved << description.magicnumber << description.memory << endl;
			return 1;
		}
		cerr << "Extracting " << description.decodedsize << " bytes with " << description.memory << " bytes of memory" << endl;
		mixer p( description.memory );
		coder c;
		cout.clear();
		c.decode( p, description.decodedsize );
		return 0;
	}
	else {
		cerr << "gdmc (C) 2009, Daniel Glasson (daniel_glasson@optusnet.com.au)" << endl;
		cerr << "To compress:   gdmcV1 -c [-m=512] <infile >outfile" << endl;
        cerr << "To extract: gdmcV1 -e <infile >outfile" << endl;
		cerr << "eg. gdmcV1 -c -m=16 <myfile.abc >mynewfile.gdmc" << endl << endl;
		return 0;
	}
}
