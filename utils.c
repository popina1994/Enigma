#include <string.h>
#include <ctype.h>
#include "utils.h"

/**
 * @brief Funkcija prebacuje hex string u binarnu reprezentaciju i pise ga u niz bin_key
 * @details Funkciji se prosledi kljuc kao hex string, kao i duzina kljuca. Posle provere da li string odgovara formatu, ona u bin_key upisuje binarnu reprezentaciju kljuca.
 * 
 * @param hex_key String sa kljucem u hex formatu. Njegova duzina mora odgovarati key_length parametru, koji je u bitovima. Npr. a6be1f duzine24.
 * @param bin_key Niz karaktera gde ce se upisati kljuc posle konverzije. Njegova velicina mora biti ista kao key_length u bitovima, inace dolazi do buffer overflow-a. Funkcija ne proverava da li je ovo ispunjeno.
 * @param key_length Duzina kljuca u bitima.
 * @return Kod greske do koje je doslo
 */
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


/**
 * @brief Prima karakter iz hex stringa i vraca 4 bita koja predstavlja.
 * @details Funkcija prima bilo koju hex cifru 0-9, a-f, A-F i vraca karakter sa kodom 0x0y u hexu, ili 0xff, ako karakter nije validan.
 * 
 * @param hex Hex cifra za konverziju.
 * @return Karakter cija su prva 4 bita 0, a ostali odredjeni argumentom hex, ili 0xff, ako karakter nije hex cifra
 */
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