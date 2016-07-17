#ifndef FILEHEADER_H_INCLUDED
#define FILEHEADER_H_INCLUDED

/*
* File header struct
* The header for the compressed file format
*/
struct fileheader {
	unsigned int magicnumber;
	unsigned int version;
	unsigned int reserved[2];
	unsigned long long memory;
	unsigned long long decodedsize;
};

#endif // FILEHEADER_H_INCLUDED
