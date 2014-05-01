#include <string.h>
#include <ctype.h>
#include "utils.h"


int hex_2_bin(const char * const hex_key, char bin_key[], int key_length)
{
	int len = strlen(hex_key);
	int i;

	if (key_length%8 != 0)
		return 1; //Standardni kljucevi su celobajtni

	if (strlen(hex_key) != key_length/4)
		return 2; //hex_key nije duzine key_length

	for (i=0;i<len;i++)
	{
		if (!((hex_key[i] >= '0' && hex_key[i] <= '9') || (hex_key[i] >= 'a' && hex_key[i] <= 'f') || (hex_key[i] >= 'A' && hex_key[i] <= 'F')))
			return 3; //U hex_key se nalaze ilegalni karakteri
	}

	for (i=0;i<len/2;i++)
	{
		bin_key[i]=(hex_2_code(hex_key[2*i])<<4)|hex_2_code(hex_key[2*i+1]);// Punimo niz odgovarajucim karakterima
	}

	return 0; // Sve je dobro proslo
}



char hex_2_code(char hex)
{
	switch(hex)
	{
		case '0': return '\x00';
		case '1': return '\x01';
		case '2': return '\x02';
		case '3': return '\x03';
		case '4': return '\x04';
		case '5': return '\x05';
		case '6': return '\x06';
		case '7': return '\x07';
		case '8': return '\x08';
		case '9': return '\x09';
		case 'a': 
		case 'A': return '\x0a';
		case 'b': 
		case 'B': return '\x0b';
		case 'c': 
		case 'C': return '\x0c';
		case 'd': 
		case 'D': return '\x0d';
		case 'e': 
		case 'E': return '\x0e';
		case 'f': 
		case 'F': return '\x0f';
		default:  return '\xff';
	}
}