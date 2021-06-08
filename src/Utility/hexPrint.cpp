#include "hexPrint.h"

char hexLookUp[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

std::string hexPrintChar(unsigned char uCharHex) {
	std::string outString;

	outString = hexLookUp[(uCharHex >> 4)];
	outString += hexLookUp[(uCharHex & 0x0F)];
	
	return outString;
}

std::string hexPrintShort(unsigned short uShortHex) {
	std::string outString;

	outString = hexLookUp[(uShortHex >> 12)];
	outString += hexLookUp[(uShortHex & 0x0F00) >> 8];
	outString += hexLookUp[(uShortHex & 0x00F0) >> 4];
	outString += hexLookUp[(uShortHex & 0x000F)];

	return outString;
}