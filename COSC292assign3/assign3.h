#ifndef ASSIGN3_H
#define ASSIGN3_H

#include <stdio.h>

typedef unsigned short WORD;
typedef unsigned int DWORD; // could also use long on Windows systems
typedef signed int LONG; // could also use long on Windows systems
typedef unsigned char BYTE;

#pragma pack(push) // Used to store the default byte alignment
#pragma pack(1) // Set the byte alignment

typedef struct
{
	// Bitmap file header - 14 bytes
	WORD wType; // Should be 'B' 'M'
	DWORD dwFileSize;
	WORD wReserved1;
	WORD wReserved2;
	DWORD dwDataOffset; // Should always be 54 bytes for our application
	// DIB header (bitmap information header)
	DWORD dwHeaderSize; // Should be 40 for our application
	LONG lWidth;
	LONG lHeight;
	WORD wPlanes; // Should be 1
	WORD wBitCount; // Should be 24 for our application
	DWORD dwCompression; // Should be 0 for our application
	DWORD dwImageSize;
	LONG lXPelsPerMeter;
	LONG lYPelsPerMeter;
	DWORD dwClrUsed; // Should be 0 for our application
	DWORD dwClrImportant;
} BITMAPHDR;

typedef struct
{
	// Strangely, blue comes first in the pixel...
	BYTE bBlu, bGrn, bRed;
} PIXEL;

// A struct that represents the entire image
typedef struct
{
	BITMAPHDR* bmHDR;
	PIXEL* bmData;
} IMAGE;

void HideInImage(IMAGE* imgPtr, FILE* filePtr);

void ExtractFileFromImage(IMAGE* imgPtr, FILE* filePtr);

unsigned int GetFileSize(FILE* filePtr);

#endif // !ASSIGN3_H