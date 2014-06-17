#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "des_public.h"

typedef enum errorsDES DES_ERROR;
/**
* @brief Proverava korektnost 64-bitnog DES ključa
* @details Svaki bajt 64-bitnog ključa treba da bude neparan, pri čemu poslednji bit čuva parnost
*
* @param key Osmobajtni niz karaktera koji sadrži ključ
* @return 0 ako je ključ nekorektan, 1 ako je korektan
*/
int isValidDESKey(unsigned char key[])
{
	int i, j;
	unsigned char mask;
	int sum;

	for (i = 0; i <8; i++)
	{
		sum = 0;
		mask = '\x01';
		for (j = 0; j<7; j++)
		{
			sum += key[i] & mask;
			mask <<= 1;
		}
		sum += key[8] & mask;
		if (sum % 2 == 0)
			return 0;
	}

	return 1;
}

/**
* @brief Izdvaja sve bitove, osim bitova parnosti, iz ključa
* @details
*
* @param char [description]
*/
void PC1DES(unsigned char input[], unsigned char key[])
{
	key[0] = (input[7] & 0x80) >> 0 | (input[6] & 0x80) >> 1 | (input[5] & 0x80) >> 2 | (input[4] & 0x80) >> 3 | (input[3] & 0x80) >> 4 | (input[2] & 0x80) >> 5 | (input[1] & 0x80) >> 6 | (input[0] & 0x80) >> 7;
	key[1] = (input[7] & 0x40) << 1 | (input[6] & 0x40) >> 0 | (input[5] & 0x40) >> 1 | (input[4] & 0x40) >> 2 | (input[3] & 0x40) >> 3 | (input[2] & 0x40) >> 4 | (input[1] & 0x40) >> 5 | (input[0] & 0x40) >> 6;
	key[2] = (input[7] & 0x20) << 2 | (input[6] & 0x20) << 1 | (input[5] & 0x20) >> 0 | (input[4] & 0x20) >> 1 | (input[3] & 0x20) >> 2 | (input[2] & 0x20) >> 3 | (input[1] & 0x20) >> 4 | (input[0] & 0x20) >> 5;
	key[3] = (input[7] & 0x10) << 3 | (input[6] & 0x10) << 2 | (input[5] & 0x10) << 1 | (input[4] & 0x10) >> 0 | (input[7] & 0x02) << 2 | (input[6] & 0x02) << 1 | (input[5] & 0x02) << 0 | (input[4] & 0x02) >> 1;
	key[4] = (input[3] & 0x02) << 6 | (input[2] & 0x02) << 5 | (input[1] & 0x02) << 4 | (input[0] & 0x02) << 3 | (input[7] & 0x04) << 1 | (input[6] & 0x04) << 0 | (input[5] & 0x04) >> 1 | (input[4] & 0x04) >> 2;
	key[5] = (input[3] & 0x04) << 5 | (input[2] & 0x04) << 4 | (input[1] & 0x04) << 3 | (input[0] & 0x04) << 2 | (input[7] & 0x08) << 0 | (input[6] & 0x08) >> 1 | (input[5] & 0x08) >> 2 | (input[4] & 0x08) >> 3;
	key[6] = (input[3] & 0x08) << 4 | (input[2] & 0x08) << 3 | (input[1] & 0x08) << 2 | (input[0] & 0x08) << 1 | (input[3] & 0x10) >> 1 | (input[2] & 0x10) >> 2 | (input[1] & 0x10) >> 3 | (input[0] & 0x10) >> 4;

	return;

}

void cirShift1(unsigned char key[], unsigned char newKey[])
{
	newKey[0] = (key[0] << 1) | (key[1] & 0x80) >> 7;
	newKey[1] = (key[1] << 1) | (key[2] & 0x80) >> 7;
	newKey[2] = (key[2] << 1) | (key[3] & 0x80) >> 7;
	newKey[3] = ((key[3] << 1) & 0xee) | (key[0] & 0x80) >> 3 | (key[4] & 0x80) >> 7;
	newKey[4] = (key[4] << 1) | (key[5] & 0x80) >> 7;
	newKey[5] = (key[5] << 1) | (key[6] & 0x80) >> 7;
	newKey[6] = (key[6] << 1) | (key[3] & 0x08) >> 3;
	return;
}

void cirShift2(unsigned char key[], unsigned char newKey[])
{
	newKey[0] = (key[0] << 2) | (key[1] & 0xc0) >> 6;
	newKey[1] = (key[1] << 2) | (key[2] & 0xc0) >> 6;
	newKey[2] = (key[2] << 2) | (key[3] & 0xc0) >> 6;
	newKey[3] = ((key[3] << 2) & 0xcc) | (key[0] & 0xc0) >> 2 | (key[4] & 0xc0) >> 6;
	newKey[4] = (key[4] << 2) | (key[5] & 0xc0) >> 6;
	newKey[5] = (key[5] << 2) | (key[6] & 0xc0) >> 6;
	newKey[6] = (key[6] << 2) | (key[3] & 0x0c) >> 2;
	return;
}

void PC2DES(unsigned char input[], unsigned char output[])
{
	output[0] = (input[1] & 0x04) << 5 | (input[2] & 0x80) >> 1 | (input[1] & 0x20) | (input[2] & 0x01) << 4 | (input[0] & 0x80) >> 4 | (input[0] & 0x08) >> 1 | (input[0] & 0x20) >> 4 | (input[3] & 0x10) >> 4;
	output[1] = (input[1] & 0x02) << 6 | (input[0] & 0x04) << 4 | (input[2] & 0x08) << 2 | (input[1] & 0x40) >> 2 | (input[2] & 0x02) << 2 | (input[2] & 0x20) >> 3 | (input[1] & 0x10) >> 3 | (input[0] & 0x10) >> 4;
	output[2] = (input[3] & 0x40) << 1 | (input[0] & 0x01) << 6 | (input[1] & 0x01) << 5 | (input[0] & 0x02) << 3 | (input[3] & 0x20) >> 2 | (input[2] & 0x10) >> 2 | (input[1] & 0x08) >> 2 | (input[0] & 0x40) >> 6;
	output[3] = (input[5] & 0x80) | (input[6] & 0x10) << 2 | (input[3] & 0x02) << 4 | (input[4] & 0x08) << 1 | (input[5] & 0x02) << 2 | (input[6] & 0x02) << 1 | (input[3] & 0x04) >> 1 | (input[4] & 0x01);
	output[4] = (input[6] & 0x20) << 2 | (input[5] & 0x08) << 3 | (input[4] & 0x80) >> 2 | (input[5] & 0x01) << 4 | (input[5] & 0x10) >> 1 | (input[6] & 0x80) >> 5 | (input[4] & 0x02) | (input[6] & 0x01);
	output[5] = (input[4] & 0x40) << 1 | (input[6] & 0x08) << 3 | (input[5] & 0x04) << 3 | (input[5] & 0x40) >> 2 | (input[6] & 0x40) >> 3 | (input[4] & 0x10) >> 2 | (input[3] & 0x08) >> 2 | (input[3] & 0x01);
	return;
}

void expandDES(unsigned char block[], unsigned char expBlock[])
{
	expBlock[0] = (block[3] & 0x01) << 7 | (block[0] & 0xf8) >> 1 | (block[0] & 0x18) >> 3;
	expBlock[1] = (block[0] & 0x07) << 5 | (block[1] & 0x80) >> 3 | (block[0] & 0x01) << 3 | (block[1] & 0xe0) >> 5;
	expBlock[2] = (block[1] & 0x18) << 3 | (block[1] & 0x1f) << 1 | (block[2] & 0x80) >> 7;
	expBlock[3] = (block[1] & 0x01) << 7 | (block[2] & 0xf8) >> 1 | (block[2] & 0x18) >> 3;
	expBlock[4] = (block[2] & 0x07) << 5 | (block[3] & 0x80) >> 3 | (block[2] & 0x01) << 3 | (block[3] & 0xe0) >> 5;
	expBlock[5] = (block[3] & 0x18) << 3 | (block[3] & 0x1f) << 1 | (block[0] & 0x80) >> 7;
	return;
}

void permutationDES(unsigned char block[], unsigned char permuted[])
{
	permuted[0] = (block[1] & 0x01) << 7 | (block[0] & 0x02) << 5 | (block[2] & 0x10) << 1 | (block[2] & 0x08) << 1 | (block[3] & 0x08) << 0 | (block[1] & 0x10) >> 2 | (block[3] & 0x10) >> 3 | (block[2] & 0x80) >> 7;
	permuted[1] = (block[0] & 0x80) << 0 | (block[1] & 0x02) << 5 | (block[2] & 0x02) << 4 | (block[3] & 0x40) >> 2 | (block[0] & 0x08) << 0 | (block[2] & 0x40) >> 4 | (block[3] & 0x02) >> 0 | (block[1] & 0x40) >> 6;
	permuted[2] = (block[0] & 0x40) << 1 | (block[0] & 0x01) << 6 | (block[2] & 0x01) << 5 | (block[1] & 0x04) << 2 | (block[3] & 0x01) << 3 | (block[3] & 0x20) >> 3 | (block[0] & 0x20) >> 4 | (block[1] & 0x80) >> 7;
	permuted[3] = (block[2] & 0x20) << 2 | (block[1] & 0x08) << 3 | (block[3] & 0x04) << 3 | (block[0] & 0x04) << 2 | (block[2] & 0x04) << 1 | (block[1] & 0x20) >> 3 | (block[0] & 0x10) >> 3 | (block[3] & 0x80) >> 7;
}

void feistelDES(unsigned char key[], unsigned char block[], unsigned char output[])
{
	static unsigned char sub[8][64] = { { 0x0e, 0x00, 0x04, 0x0f, 0x0d, 0x07, 0x01, 0x04, 0x02, 0x0e, 0x0f, 0x02, 0x0b, 0x0d, 0x08, 0x01, 0x03, 0x0a, 0x0a, 0x06, 0x06, 0x0c, 0x0c, 0x0b, 0x05, 0x09, 0x09, 0x05, 0x00, 0x03, 0x07, 0x08, 0x04, 0x0f, 0x01, 0x0c, 0x0e, 0x08, 0x08, 0x02, 0x0d, 0x04, 0x06, 0x09, 0x02, 0x01, 0x0b, 0x07, 0x0f, 0x05, 0x0c, 0x0b, 0x09, 0x03, 0x07, 0x0e, 0x03, 0x0a, 0x0a, 0x00, 0x05, 0x06, 0x00, 0x0d },
	{ 0x0f, 0x03, 0x01, 0x0d, 0x08, 0x04, 0x0e, 0x07, 0x06, 0x0f, 0x0b, 0x02, 0x03, 0x08, 0x04, 0x0e, 0x09, 0x0c, 0x07, 0x00, 0x02, 0x01, 0x0d, 0x0a, 0x0c, 0x06, 0x00, 0x09, 0x05, 0x0b, 0x0a, 0x05, 0x00, 0x0d, 0x0e, 0x08, 0x07, 0x0a, 0x0b, 0x01, 0x0a, 0x03, 0x04, 0x0f, 0x0d, 0x04, 0x01, 0x02, 0x05, 0x0b, 0x08, 0x06, 0x0c, 0x07, 0x06, 0x0c, 0x09, 0x00, 0x03, 0x05, 0x02, 0x0e, 0x0f, 0x09 },
	{ 0x0a, 0x0d, 0x00, 0x07, 0x09, 0x00, 0x0e, 0x09, 0x06, 0x03, 0x03, 0x04, 0x0f, 0x06, 0x05, 0x0a, 0x01, 0x02, 0x0d, 0x08, 0x0c, 0x05, 0x07, 0x0e, 0x0b, 0x0c, 0x04, 0x0b, 0x02, 0x0f, 0x08, 0x01, 0x0d, 0x01, 0x06, 0x0a, 0x04, 0x0d, 0x09, 0x00, 0x08, 0x06, 0x0f, 0x09, 0x03, 0x08, 0x00, 0x07, 0x0b, 0x04, 0x01, 0x0f, 0x02, 0x0e, 0x0c, 0x03, 0x05, 0x0b, 0x0a, 0x05, 0x0e, 0x02, 0x07, 0x0c },
	{ 0x07, 0x0d, 0x0d, 0x08, 0x0e, 0x0b, 0x03, 0x05, 0x00, 0x06, 0x06, 0x0f, 0x09, 0x00, 0x0a, 0x03, 0x01, 0x04, 0x02, 0x07, 0x08, 0x02, 0x05, 0x0c, 0x0b, 0x01, 0x0c, 0x0a, 0x04, 0x0e, 0x0f, 0x09, 0x0a, 0x03, 0x06, 0x0f, 0x09, 0x00, 0x00, 0x06, 0x0c, 0x0a, 0x0b, 0x01, 0x07, 0x0d, 0x0d, 0x08, 0x0f, 0x09, 0x01, 0x04, 0x03, 0x05, 0x0e, 0x0b, 0x05, 0x0c, 0x02, 0x07, 0x08, 0x02, 0x04, 0x0e },
	{ 0x02, 0x0e, 0x0c, 0x0b, 0x04, 0x02, 0x01, 0x0c, 0x07, 0x04, 0x0a, 0x07, 0x0b, 0x0d, 0x06, 0x01, 0x08, 0x05, 0x05, 0x00, 0x03, 0x0f, 0x0f, 0x0a, 0x0d, 0x03, 0x00, 0x09, 0x0e, 0x08, 0x09, 0x06, 0x04, 0x0b, 0x02, 0x08, 0x01, 0x0c, 0x0b, 0x07, 0x0a, 0x01, 0x0d, 0x0e, 0x07, 0x02, 0x08, 0x0d, 0x0f, 0x06, 0x09, 0x0f, 0x0c, 0x00, 0x05, 0x09, 0x06, 0x0a, 0x03, 0x04, 0x00, 0x05, 0x0e, 0x03 },
	{ 0x0c, 0x0a, 0x01, 0x0f, 0x0a, 0x04, 0x0f, 0x02, 0x09, 0x07, 0x02, 0x0c, 0x06, 0x09, 0x08, 0x05, 0x00, 0x06, 0x0d, 0x01, 0x03, 0x0d, 0x04, 0x0e, 0x0e, 0x00, 0x07, 0x0b, 0x05, 0x03, 0x0b, 0x08, 0x09, 0x04, 0x0e, 0x03, 0x0f, 0x02, 0x05, 0x0c, 0x02, 0x09, 0x08, 0x05, 0x0c, 0x0f, 0x03, 0x0a, 0x07, 0x0b, 0x00, 0x0e, 0x04, 0x01, 0x0a, 0x07, 0x01, 0x06, 0x0d, 0x00, 0x0b, 0x08, 0x06, 0x0d },
	{ 0x04, 0x0d, 0x0b, 0x00, 0x02, 0x0b, 0x0e, 0x07, 0x0f, 0x04, 0x00, 0x09, 0x08, 0x01, 0x0d, 0x0a, 0x03, 0x0e, 0x0c, 0x03, 0x09, 0x05, 0x07, 0x0c, 0x05, 0x02, 0x0a, 0x0f, 0x06, 0x08, 0x01, 0x06, 0x01, 0x06, 0x04, 0x0b, 0x0b, 0x0d, 0x0d, 0x08, 0x0c, 0x01, 0x03, 0x04, 0x07, 0x0a, 0x0e, 0x07, 0x0a, 0x09, 0x0f, 0x05, 0x06, 0x00, 0x08, 0x0f, 0x00, 0x0e, 0x05, 0x02, 0x09, 0x03, 0x02, 0x0c },
	{ 0x0d, 0x01, 0x02, 0x0f, 0x08, 0x0d, 0x04, 0x08, 0x06, 0x0a, 0x0f, 0x03, 0x0b, 0x07, 0x01, 0x04, 0x0a, 0x0c, 0x09, 0x05, 0x03, 0x06, 0x0e, 0x0b, 0x05, 0x00, 0x00, 0x0e, 0x0c, 0x09, 0x07, 0x02, 0x07, 0x02, 0x0b, 0x01, 0x04, 0x0e, 0x01, 0x07, 0x09, 0x04, 0x0c, 0x0a, 0x0e, 0x08, 0x02, 0x0d, 0x00, 0x0f, 0x06, 0x0c, 0x0a, 0x09, 0x0d, 0x00, 0x0f, 0x03, 0x03, 0x05, 0x05, 0x06, 0x08, 0x0b } };
	unsigned char xored[6];
	int i;
	expandDES(block, xored);
	for (i = 0; i < 6; i++)
	{
		xored[i] = key[i] ^ xored[i];
	}

	output[0] = (sub[0][(xored[0] & 0xfc) >> 2] << 4) | (sub[1][(xored[0] & 0x03) << 4 | (xored[1] & 0xf0) >> 4]);
	output[1] = (sub[2][(xored[1] & 0x0f) << 2 | (xored[2] & 0xc0) >> 6] << 4) | sub[3][xored[2] & 0x3f];
	output[2] = (sub[4][(xored[3] & 0xfc) >> 2] << 4) | (sub[5][(xored[3] & 0x03) << 4 | (xored[4] & 0xf0) >> 4]);
	output[3] = (sub[6][(xored[4] & 0x0f) << 2 | (xored[5] & 0xc0) >> 6] << 4) | sub[7][xored[5] & 0x3f];

	permutationDES(output, xored);
	for (i = 0; i < 4; i++)
	{
		output[i] = xored[i];
	}
}

void IPDES(unsigned char block[], unsigned char newBlock[])
{
	newBlock[0] = (block[7] & 0x40) << 1 | (block[6] & 0x40) << 0 | (block[5] & 0x40) >> 1 | (block[4] & 0x40) >> 2 | (block[3] & 0x40) >> 3 | (block[2] & 0x40) >> 4 | (block[1] & 0x40) >> 5 | (block[0] & 0x40) >> 6;
	newBlock[1] = (block[7] & 0x10) << 3 | (block[6] & 0x10) << 2 | (block[5] & 0x10) << 1 | (block[4] & 0x10) >> 0 | (block[3] & 0x10) >> 1 | (block[2] & 0x10) >> 2 | (block[1] & 0x10) >> 3 | (block[0] & 0x10) >> 4;
	newBlock[2] = (block[7] & 0x04) << 5 | (block[6] & 0x04) << 4 | (block[5] & 0x04) << 3 | (block[4] & 0x04) << 2 | (block[3] & 0x04) << 1 | (block[2] & 0x04) << 0 | (block[1] & 0x04) >> 1 | (block[0] & 0x04) >> 2;
	newBlock[3] = (block[7] & 0x01) << 7 | (block[6] & 0x01) << 6 | (block[5] & 0x01) << 5 | (block[4] & 0x01) << 4 | (block[3] & 0x01) << 3 | (block[2] & 0x01) << 2 | (block[1] & 0x01) << 1 | (block[0] & 0x01) << 0;
	newBlock[4] = (block[7] & 0x80) << 0 | (block[6] & 0x80) >> 1 | (block[5] & 0x80) >> 2 | (block[4] & 0x80) >> 3 | (block[3] & 0x80) >> 4 | (block[2] & 0x80) >> 5 | (block[1] & 0x80) >> 6 | (block[0] & 0x80) >> 7;
	newBlock[5] = (block[7] & 0x20) << 2 | (block[6] & 0x20) << 1 | (block[5] & 0x20) >> 0 | (block[4] & 0x20) >> 1 | (block[3] & 0x20) >> 2 | (block[2] & 0x20) >> 3 | (block[1] & 0x20) >> 4 | (block[0] & 0x20) >> 5;
	newBlock[6] = (block[7] & 0x08) << 4 | (block[6] & 0x08) << 3 | (block[5] & 0x08) << 2 | (block[4] & 0x08) << 1 | (block[3] & 0x08) << 0 | (block[2] & 0x08) >> 1 | (block[1] & 0x08) >> 2 | (block[0] & 0x08) >> 3;
	newBlock[7] = (block[7] & 0x02) << 6 | (block[6] & 0x02) << 5 | (block[5] & 0x02) << 4 | (block[4] & 0x02) << 3 | (block[3] & 0x02) << 2 | (block[2] & 0x02) << 1 | (block[1] & 0x02) << 0 | (block[0] & 0x02) >> 1;
	return;
}

void FPDES(unsigned char block[], unsigned char newBlock[])
{
	newBlock[0] = (block[4] & 0x01) << 7 | (block[0] & 0x01) << 6 | (block[5] & 0x01) << 5 | (block[1] & 0x01) << 4 | (block[6] & 0x01) << 3 | (block[2] & 0x01) << 2 | (block[7] & 0x01) << 1 | (block[3] & 0x01) << 0;
	newBlock[1] = (block[4] & 0x02) << 6 | (block[0] & 0x02) << 5 | (block[5] & 0x02) << 4 | (block[1] & 0x02) << 3 | (block[6] & 0x02) << 2 | (block[2] & 0x02) << 1 | (block[7] & 0x02) << 0 | (block[3] & 0x02) >> 1;
	newBlock[2] = (block[4] & 0x04) << 5 | (block[0] & 0x04) << 4 | (block[5] & 0x04) << 3 | (block[1] & 0x04) << 2 | (block[6] & 0x04) << 1 | (block[2] & 0x04) << 0 | (block[7] & 0x04) >> 1 | (block[3] & 0x04) >> 2;
	newBlock[3] = (block[4] & 0x08) << 4 | (block[0] & 0x08) << 3 | (block[5] & 0x08) << 2 | (block[1] & 0x08) << 1 | (block[6] & 0x08) << 0 | (block[2] & 0x08) >> 1 | (block[7] & 0x08) >> 2 | (block[3] & 0x08) >> 3;
	newBlock[4] = (block[4] & 0x10) << 3 | (block[0] & 0x10) << 2 | (block[5] & 0x10) << 1 | (block[1] & 0x10) << 0 | (block[6] & 0x10) >> 1 | (block[2] & 0x10) >> 2 | (block[7] & 0x10) >> 3 | (block[3] & 0x10) >> 4;
	newBlock[5] = (block[4] & 0x20) << 2 | (block[0] & 0x20) << 1 | (block[5] & 0x20) << 0 | (block[1] & 0x20) >> 1 | (block[6] & 0x20) >> 2 | (block[2] & 0x20) >> 3 | (block[7] & 0x20) >> 4 | (block[3] & 0x20) >> 5;
	newBlock[6] = (block[4] & 0x40) << 1 | (block[0] & 0x40) << 0 | (block[5] & 0x40) >> 1 | (block[1] & 0x40) >> 2 | (block[6] & 0x40) >> 3 | (block[2] & 0x40) >> 4 | (block[7] & 0x40) >> 5 | (block[3] & 0x40) >> 6;
	newBlock[7] = (block[4] & 0x80) << 0 | (block[0] & 0x80) >> 1 | (block[5] & 0x80) >> 2 | (block[1] & 0x80) >> 3 | (block[6] & 0x80) >> 4 | (block[2] & 0x80) >> 5 | (block[7] & 0x80) >> 6 | (block[3] & 0x80) >> 7;
	return;
}

void subkeysDES(unsigned char key[], unsigned char subkeys[16][6])
{
	unsigned char tempKey[8] = { 0 };
	unsigned char tempKey2[8] = { 0 };


	PC1DES(key, tempKey);

	cirShift1(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[0]);

	cirShift1(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[1]);

	cirShift2(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[2]);

	cirShift2(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[3]);

	cirShift2(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[4]);

	cirShift2(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[5]);

	cirShift2(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[6]);

	cirShift2(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[7]);

	cirShift1(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[8]);

	cirShift2(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[9]);

	cirShift2(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[10]);

	cirShift2(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[11]);

	cirShift2(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[12]);

	cirShift2(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[13]);

	cirShift2(tempKey, tempKey2);
	PC2DES(tempKey2, subkeys[14]);

	cirShift1(tempKey2, tempKey);
	PC2DES(tempKey, subkeys[15]);
	return;
}

void encryptBlockDES(unsigned char subkeys[16][6], unsigned char block[], unsigned char output[])
{
	unsigned char buffer[8];
	unsigned char * left = output;
	unsigned char * right = output + 4;
	unsigned char * left1 = buffer;
	unsigned char * right1 = buffer + 4;
	int i, j;

	IPDES(block, output);

	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 4; j++)
			left1[j] = right[j];

		feistelDES(subkeys[i], right, right1);
		for (j = 0; j < 4; j++)
			right1[j] = right1[j] ^ left[j];

		for (j = 0; j < 8; j++)
			output[j] = buffer[j];
	}

	buffer[0] = output[4];
	buffer[1] = output[5];
	buffer[2] = output[6];
	buffer[3] = output[7];
	buffer[4] = output[0];
	buffer[5] = output[1];
	buffer[6] = output[2];
	buffer[7] = output[3];

	FPDES(buffer, output);
}

void decryptBlockDES(unsigned char subkeys[16][6], unsigned char block[], unsigned char output[])
{
	unsigned char buffer[8];
	unsigned char * left = output;
	unsigned char * right = output + 4;
	unsigned char * left1 = buffer;
	unsigned char * right1 = buffer + 4;
	int i, j;

	IPDES(block, output);

	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 4; j++)
			left1[j] = right[j];

		feistelDES(subkeys[15 - i], right, right1);

		for (j = 0; j < 4; j++)
			right1[j] = right1[j] ^ left[j];

		for (j = 0; j < 8; j++)
			output[j] = buffer[j];
	}

	buffer[0] = output[4];
	buffer[1] = output[5];
	buffer[2] = output[6];
	buffer[3] = output[7];
	buffer[4] = output[0];
	buffer[5] = output[1];
	buffer[6] = output[2];
	buffer[7] = output[3];

	FPDES(buffer, output);
}



uint32_t crc32(uint32_t crc, const void *buf, size_t size)
{
	static uint32_t crc32_tab[] = {
		0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
		0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
		0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
		0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
		0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
		0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
		0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
		0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
		0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
		0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
		0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
		0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
		0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
		0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
		0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
		0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
		0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
		0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
		0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
		0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
		0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
		0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
		0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
		0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
		0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
		0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
		0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
		0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
		0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
		0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
		0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
		0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
		0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
		0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
		0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
		0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
		0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
		0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
		0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
		0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
		0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
		0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
		0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
	};
	const uint8_t *p;

	p = buf;
	crc = crc ^ ~0U;

	while (size--)
		crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);

	return crc ^ ~0U;
}


DES_ERROR encryptDESECB(char* inputName, char* outputName, unsigned char key[])
{
	/*Format enkriptovan fajla je CRC+4*0(8 bajtova) Naziv(256 bajtova) Enkriptovan fajl sa padding-om na kraju (ostatak)*/


	unsigned char subkeys[16][6];//Kljucevi za runde
	unsigned char block[8];//Blok za enkripciju
	unsigned char output[8];//Enkriptovan blok
	FILE* input;//Ulazni fajl
	FILE* izlaz;//Izlazni fajl
	int i = 0;
	int temp;
	int j;
	uint32_t crc = 0;//CRC kod
	unsigned char name[256] = {0};//Ime ulaznog fajla

	if (!isValidDESKey(key))
	{
		return INVALID_KEY;
	}

	if (strlen(inputName) > 255)//U Windowsu je ogranicenje 255 karaktera + NULL
	{
		return NAME_TOO_LONG;
	}

	strcpy(name, inputName);//Prekopiraj ime pre enkripciju
	crc = crc32(crc, name, 256);//Izracunaj CRC

	input = fopen(inputName, "rb");//Otvori ulaznu datoteku
	if (input == NULL)
	{
		return INPUT_FILE;//Problem sa otvaranjem ulaza
	}

	izlaz = fopen(outputName, "wb");//Otvori izlaznu datoteku
	if (izlaz == NULL)
	{
		fclose(input);
		return OUTPUT_FILE;//problem sa otvaranjem izlaza
	}

	subkeysDES(key, subkeys);//Izracunaj Kljuceve za razlicite runde

	while ((temp = fread(block, sizeof(char), 8, input)) != 0)//Citaj blok po blok i racunaj CRC
	{
		crc = crc32(crc, block, temp);
	}


	rewind(input);//Vrati na pocetak

	block[0] = ((unsigned int)crc >> 24);//Spremi CRC za enkripciju
	block[1] = ((unsigned int)crc >> 16);
	block[2] = ((unsigned int)crc >> 8);
	block[3] = (unsigned int)crc;


	block[4] = block[5] = block[6] = block[7] = 0;//Preostala 4 bajta su popuna


	encryptBlockDES(subkeys, block, output);//Enkriptuj CRC
	if (!fwrite(output, sizeof(char), 8, izlaz))//Pisi CRC
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	for (int i = 0; i < 256; i += 8)//Citaj ime
	{
		encryptBlockDES(subkeys, name+i, output);//Enkriptuj ga
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	while ((temp = fread(block, sizeof(char), 8, input)) == 8)//Citaj fajl
	{
		encryptBlockDES(subkeys, block, output);//Enkriptuj ga
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	j = 8 - temp;//Koliko treba karaktera za padding
	for (; temp < 8; temp++)//Dopuni padding-om
	{
		block[temp] = j;
	}

	encryptBlockDES(subkeys, block, output);//Enkriptuj
	if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi u datoteku
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	fclose(izlaz);//Zatvori izlaznu datoteku
	fclose(input);//Zatvori ulaznu datoteku
	return NO_ERROR;
}


DES_ERROR decryptDESECB(char * inputName, char* outputName, unsigned char key[])
{
	unsigned char subkeys[16][6];//Kljucevi za podrunde
	unsigned char block[8];//Blok sifrovanog teksta
	unsigned char output[8];//Blok desifrovanog teksta
	unsigned char name[256];//Naziv dekriptovanog fajla
	unsigned char randomNo[3];//Slucajan broj za modifikaciju naziva
	uint32_t crc = 0;//CRC koji generisemo iz dekriptovanog fajla
	uint32_t crc_file;//CRC koji citamo iz dekriptovanog fajla
	FILE* izlaz;//Ispis
	FILE* input;//Citanje
	unsigned int i = 0;//Brojac
	unsigned int size;//Velicina fajla

	if (!isValidDESKey(key))
	{
		return INVALID_KEY;
	}

	input = fopen(inputName, "rb");
	if (input == NULL)
		return INPUT_FILE;//Greska pri otvaranju ulaznog fajla

	if (fseek(input, 0, SEEK_END))
	{
		fclose(input);
		return SEEK;//Greska pri premotavanju
	}

	size = ftell(input) + 1;//Velicina fajla
	subkeysDES(key, subkeys);//Prosirivanje kljuca
	rewind(input);//Premotaj fajl

	fread(block, sizeof(char), 8, input);//Ucitaj CRC
	decryptBlockDES(subkeys, block, output);//Dekriptuj CRC

	crc_file = output[0] << 24 | output[1] << 16 | output[2] << 8 | output[3];//Upamti CRC

	i = 8;
	while (i < size-16 )//Prodji kroz datoteku
	{
		fread(block, sizeof(char), 8, input);
		i+=8;
		decryptBlockDES(subkeys, block, output);
		crc = crc32(crc, output, 8);//Racunaj CRC
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys, block, output);
	i = output[7];
	crc = crc32(crc,output,8 - i);//Racunaj CRC

	if (crc != crc_file)
	{
		return CRC_MISMATCH;//CRCovi se ne poklapaju
	}

	
	rewind(input);//Premotaj

	fread(block, sizeof(char), 8, input);//CRC

	for (i = 0; i < 256; i += 8)//Dekriptuj naziv
	{
		fread(block, sizeof(char), 8, input);
		decryptBlockDES(subkeys, block, name+i);
	}


	izlaz = fopen(name, "rb");//Proveri postoji li file datog naziva
	if (izlaz == NULL)//Ako ne postoji, pisi
	{
		izlaz = fopen(name, "wb");
		if (izlaz == NULL)
		{
			fclose(input);
			return OUTPUT_FILE;
		}
	}
	else
	{
		fclose(izlaz);//Ako postoji, dodaj na kraj slucajan dvocifren broj i pisi
		_itoa(rand() % 100, randomNo, 10);
		name[254] = '\0';
		strcat(name, randomNo);
		strcpy(outputName, name);
		izlaz = fopen(name, "wb");
		if (izlaz == NULL)
		{
			fclose(input);
			return OUTPUT_FILE;
		}
	}

	i = 256 + 8;

	while (i < size-16)//Dekriptuj datoteku
	{
		fread(block, sizeof(char), 8, input);
		i+=8;
		decryptBlockDES(subkeys, block, output);
		fwrite(output, sizeof(char), 8, izlaz);
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys, block, output);//Dekriptuj
	i = output[7];
	fwrite(output, sizeof(char), 8 - i, izlaz);
	fclose(izlaz);//Zatvori datoteke
	fclose(input);
	return NO_ERROR;
}


DES_ERROR encrypt3DESECB(char* inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[])
{

	unsigned char subkeys1[16][6];//Kljucevi za runde
	unsigned char subkeys2[16][6];
	unsigned char subkeys3[16][6];
	unsigned char block[8];//Blok za enkripciju
	unsigned char output[8];//Enkriptovan blok
	unsigned char tempb[8];
	FILE* input;//Ulazni fajl
	FILE* izlaz;//Izlazni fajl
	int i = 0;
	int temp;
	int j;
	uint32_t crc = 0;//CRC kod
	unsigned char name[256] = { 0 };//Ime ulaznog fajla

	if (!isValidDESKey(key1))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key2))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key3))
	{
		return INVALID_KEY;
	}

	if (strlen(inputName) > 255)//U Windowsu je ogranicenje 255 karaktera + NULL
	{
		return NAME_TOO_LONG;
	}

	strcpy(name, inputName);//Prekopiraj ime pre enkripciju
	crc = crc32(crc, name, 256);//Izracunaj CRC

	input = fopen(inputName, "rb");//Otvori ulaznu datoteku
	if (input == NULL)
	{
		return INPUT_FILE;//Problem sa otvaranjem ulaza
	}

	izlaz = fopen(outputName, "wb");//Otvori izlaznu datoteku
	if (izlaz == NULL)
	{
		fclose(input);
		return 3;//problem sa otvaranjem izlaza
	}

	subkeysDES(key1, subkeys1);//Izracunaj kljuceve za razlicite runde
	subkeysDES(key2, subkeys2);
	subkeysDES(key3, subkeys3);

	while ((temp = fread(block, sizeof(char), 8, input)) != 0)//Citaj blok po blok i racunaj CRC
	{
		crc = crc32(crc, block, temp);
	}


	rewind(input);//Vrati na pocetak

	block[0] = ((unsigned int)crc >> 24);//Spremi CRC za enkripciju
	block[1] = ((unsigned int)crc >> 16);
	block[2] = ((unsigned int)crc >> 8);
	block[3] = (unsigned int)crc;


	block[4] = block[5] = block[6] = block[7] = 0;//Preostala 4 bajta su popuna


	encryptBlockDES(subkeys1, block, output);//Enkriptuj CRC
	decryptBlockDES(subkeys2, output, tempb);
	encryptBlockDES(subkeys3, tempb, output);

	if (!fwrite(output, sizeof(char), 8, izlaz))//Pisi CRC
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	for (int i = 0; i < 256; i += 8)//Citaj ime
	{
		encryptBlockDES(subkeys1, name + i, output);//Enkriptuj ga
		decryptBlockDES(subkeys2, output, tempb);
		encryptBlockDES(subkeys3, tempb, output);
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	while ((temp = fread(block, sizeof(char), 8, input)) == 8)//Citaj fajl
	{
		encryptBlockDES(subkeys1, block, output);//Enkriptuj ga
		decryptBlockDES(subkeys2, output, tempb);
		encryptBlockDES(subkeys3, tempb, output);
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	j = 8 - temp;//Koliko treba karaktera za padding
	for (; temp < 8; temp++)//Dopuni padding-om
	{
		block[temp] = j;
	}

	encryptBlockDES(subkeys1, block, output);//Enkriptuj
	decryptBlockDES(subkeys2, output, tempb);
	encryptBlockDES(subkeys3, tempb, output);
	if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi u datoteku
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	fclose(izlaz);//Zatvori izlaznu datoteku
	fclose(input);//Zatvori ulaznu datoteku
	return NO_ERROR;
}


DES_ERROR decrypt3DESECB(char * inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[])
{
	unsigned char subkeys1[16][6];//Kljucevi za podrunde
	unsigned char subkeys2[16][6];
	unsigned char subkeys3[16][6];
	unsigned char block[8];//Blok sifrovanog teksta
	unsigned char output[8];//Blok desifrovanog teksta
	unsigned char tempb[8];
	unsigned char name[256];//Naziv dekriptovanog fajla
	unsigned char randomNo[3];//Slucajan broj za modifikaciju naziva
	uint32_t crc = 0;//CRC koji generisemo iz dekriptovanog fajla
	uint32_t crc_file;//CRC koji citamo iz dekriptovanog fajla
	FILE* izlaz;//Ispis
	FILE* input;//Citanje
	unsigned int i = 0;//Brojac
	unsigned int size;//Velicina fajla

	if (!isValidDESKey(key1))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key2))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key3))
	{
		return INVALID_KEY;
	}

	input = fopen(inputName, "rb");
	if (input == NULL)
		return INPUT_FILE;//Greska pri otvaranju ulaznog fajla

	if (fseek(input, 0, SEEK_END))
	{
		fclose(input);
		return SEEK;//Greska pri premotavanju
	}

	size = ftell(input) + 1;//Velicina fajla
	subkeysDES(key1, subkeys1);//Prosirivanje kljuca
	subkeysDES(key2, subkeys2);
	subkeysDES(key3, subkeys3);
	rewind(input);//Premotaj fajl

	fread(block, sizeof(char), 8, input);//Ucitaj CRC
	decryptBlockDES(subkeys1, block, output);//Dekriptuj CRC
	encryptBlockDES(subkeys2, output, tempb);
	decryptBlockDES(subkeys3, tempb, output);

	crc_file = output[0] << 24 | output[1] << 16 | output[2] << 8 | output[3];//Upamti CRC

	i = 8;
	while (i < size - 16)//Prodji kroz datoteku
	{
		fread(block, sizeof(char), 8, input);
		i += 8;
		decryptBlockDES(subkeys1, block, output);
		encryptBlockDES(subkeys2, output, tempb);
		decryptBlockDES(subkeys3, tempb, output);
		crc = crc32(crc, output, 8);//Racunaj CRC
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys1, block, output);
	encryptBlockDES(subkeys2, output, tempb);
	decryptBlockDES(subkeys3, tempb, output);
	i = output[7];
	crc = crc32(crc, output, 8 - i);//Racunaj CRC

	if (crc != crc_file)
	{
		return CRC_MISMATCH;//CRCovi se ne poklapaju
	}


	rewind(input);//Premotaj

	fread(block, sizeof(char), 8, input);//CRC

	for (i = 0; i < 256; i += 8)//Dekriptuj naziv
	{
		fread(block, sizeof(char), 8, input);
		decryptBlockDES(subkeys1, block, name + i);
		encryptBlockDES(subkeys2, output, tempb);
		decryptBlockDES(subkeys3, tempb, output);
	}


	izlaz = fopen(name, "rb");//Proveri postoji li file datog naziva
	if (izlaz == NULL)//Ako ne postoji, pisi
	{
		izlaz = fopen(name, "wb");
	}
	else
	{
		fclose(izlaz);//Ako postoji, dodaj na kraj slucajan dvocifren broj i pisi
		_itoa(rand() % 100, randomNo, 10);
		name[254] = '\0';
		strcat(name, randomNo);
		strcpy(outputName, name);
		izlaz = fopen(name, "wb");
	}

	i = 256 + 8;

	while (i < size - 16)//Dekriptuj datoteku
	{
		fread(block, sizeof(char), 8, input);
		i += 8;
		decryptBlockDES(subkeys1, block, output);
		encryptBlockDES(subkeys2, output, tempb);
		decryptBlockDES(subkeys3, tempb, output);
		fwrite(output, sizeof(char), 8, izlaz);
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys1, block, output);//Dekriptuj
	encryptBlockDES(subkeys2, output, tempb);
	decryptBlockDES(subkeys3, tempb, output);
	i = output[7];
	fwrite(output, sizeof(char), 8 - i, izlaz);
	fclose(izlaz);//Zatvori datoteke
	fclose(input);
	return NO_ERROR;
}





DES_ERROR encryptDESCBC(char* inputName, char* outputName, unsigned char key[], unsigned char iv[])
{
	/*Format enkriptovan fajla je IV+CRC+4*0(8 bajtova) Naziv(256 bajtova) Enkriptovan fajl sa padding-om na kraju (ostatak)*/

	unsigned char subkeys[16][6];//Kljucevi za runde
	unsigned char block[8];//Blok za enkripciju
	unsigned char output[8];//Enkriptovan blok
	FILE* input;//Ulazni fajl
	FILE* izlaz;//Izlazni fajl
	int i = 0,j;
	int temp;
	uint32_t crc = 0;//CRC kod
	unsigned char name[256] = { 0 };//Ime ulaznog fajla

	output[0] = iv[0];
	output[1] = iv[1];
	output[2] = iv[2];
	output[3] = iv[3];
	output[4] = iv[4];
	output[5] = iv[5];
	output[6] = iv[6];
	output[7] = iv[7];

	if (!isValidDESKey(key))
	{
		return INVALID_KEY;
	}

	if (strlen(inputName) > 255)//U Windowsu je ogranicenje 255 karaktera + NULL
	{
		return NAME_TOO_LONG;
	}

	strcpy(name, inputName);//Prekopiraj ime pre enkripciju
	crc = crc32(crc, name, 256);//Izracunaj CRC

	input = fopen(inputName, "rb");//Otvori ulaznu datoteku
	if (input == NULL)
	{
		return INPUT_FILE;//Problem sa otvaranjem ulaza
	}

	izlaz = fopen(outputName, "wb");//Otvori izlaznu datoteku
	if (izlaz == NULL)
	{
		fclose(input);
		return OUTPUT_FILE;//problem sa otvaranjem izlaza
	}

	subkeysDES(key, subkeys);//Izracunaj Kljuceve za razlicite runde

	while ((temp = fread(block, sizeof(char), 8, input)) != 0)//Citaj blok po blok i racunaj CRC
	{
		crc = crc32(crc, block, temp);
	}


	rewind(input);//Vrati na pocetak

	block[0] = ((unsigned int)crc >> 24);//Spremi CRC za enkripciju
	block[1] = ((unsigned int)crc >> 16);
	block[2] = ((unsigned int)crc >> 8);
	block[3] = (unsigned int)crc;


	block[4] = block[5] = block[6] = block[7] = 0;//Preostala 4 bajta su popuna

	for (j = 0; j < 8; j++)
	{
		block[j] = block[j] ^ output[j];
	}

	if (!fwrite(iv, sizeof(char), 8, izlaz))//Pisi CRC
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	encryptBlockDES(subkeys, block, output);//Enkriptuj CRC
	if (!fwrite(output, sizeof(char), 8, izlaz))//Pisi CRC
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	for (int i = 0; i < 256; i += 8)//Citaj ime
	{
		for (j = 0; j < 8; j++)
		{
			block[j] = name[i+j] ^ output[j];
		}
		encryptBlockDES(subkeys, block, output);//Enkriptuj ga
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	while ((temp = fread(block, sizeof(char), 8, input)) == 8)//Citaj fajl
	{
		for (j = 0; j < 8; j++)
		{
			block[j] = block[j] ^ output[j];
		}
		encryptBlockDES(subkeys, block, output);//Enkriptuj ga
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	j = 8 - temp;//Koliko treba karaktera za padding
	for (; temp < 8; temp++)//Dopuni padding-om
	{
		block[temp] = j;
	}

	for (j = 0; j < 8; j++)
	{
		block[j] = block[j] ^ output[j];
	}

	encryptBlockDES(subkeys, block, output);//Enkriptuj
	if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi u datoteku
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	fclose(izlaz);//Zatvori izlaznu datoteku
	fclose(input);//Zatvori ulaznu datoteku
	return NO_ERROR;
}


DES_ERROR decryptDESCBC(char * inputName, char* outputName, unsigned char key[])
{
	unsigned char subkeys[16][6];//Kljucevi za podrunde
	unsigned char block[8];//Blok sifrovanog teksta
	unsigned char output[8];//Blok desifrovanog teksta
	unsigned char xorblock[8];
	unsigned char name[256];//Naziv dekriptovanog fajla
	unsigned char randomNo[3];//Slucajan broj za modifikaciju naziva
	uint32_t crc = 0;//CRC koji generisemo iz dekriptovanog fajla
	uint32_t crc_file;//CRC koji citamo iz dekriptovanog fajla
	FILE* izlaz;//Ispis
	FILE* input;//Citanje
	unsigned int i = 0, j;//Brojac
	unsigned int size;//Velicina fajla

	if (!isValidDESKey(key))
	{
		return INVALID_KEY;
	}

	input = fopen(inputName, "rb");
	if (input == NULL)
		return INPUT_FILE;//Greska pri otvaranju ulaznog fajla

	if (fseek(input, 0, SEEK_END))
	{
		fclose(input);
		return SEEK;//Greska pri premotavanju
	}

	

	size = ftell(input) + 1;//Velicina fajla
	subkeysDES(key, subkeys);//Prosirivanje kljuca
	rewind(input);//Premotaj fajl

	fread(xorblock, sizeof(char), 8, input);
	fread(block, sizeof(char), 8, input);//Ucitaj CRC
	decryptBlockDES(subkeys, block, output);//Dekriptuj CRC

	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}

	crc_file = output[0] << 24 | output[1] << 16 | output[2] << 8 | output[3];//Upamti CRC

	i = 16;
	while (i < size - 16)//Prodji kroz datoteku
	{
		fread(block, sizeof(char), 8, input);
		i += 8;
		decryptBlockDES(subkeys, block, output);
		for (j = 0; j < 8; j++)
		{
			output[j] = output[j] ^ xorblock[j];
			xorblock[j] = block[j];
		}
		crc = crc32(crc, output, 8);//Racunaj CRC
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys, block, output);
	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}
	i = output[7];
	crc = crc32(crc, output, 8 - i);//Racunaj CRC

	if (crc != crc_file)
	{
		return CRC_MISMATCH;//CRCovi se ne poklapaju
	}


	rewind(input);//Premotaj

	fread(xorblock, sizeof(char), 8, input);
	fread(block, sizeof(char), 8, input);//CRC
	decryptBlockDES(subkeys, block, output);//Dekriptuj CRC

	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}

	for (i = 0; i < 256; i += 8)//Dekriptuj naziv
	{
		fread(block, sizeof(char), 8, input);
		decryptBlockDES(subkeys, block, name + i);
		for (j = 0; j < 8; j++)
		{
			name[i+j] = name[i+j] ^ xorblock[j];
			xorblock[j] = block[j];
		}
	}


	izlaz = fopen(name, "rb");//Proveri postoji li file datog naziva
	if (izlaz == NULL)//Ako ne postoji, pisi
	{
		izlaz = fopen(name, "wb");
		if (izlaz == NULL)
		{
			fclose(input);
			return OUTPUT_FILE;
		}
	}
	else
	{
		fclose(izlaz);//Ako postoji, dodaj na kraj slucajan dvocifren broj i pisi
		_itoa(rand() % 100, randomNo, 10);
		name[254] = '\0';
		strcat(name, randomNo);
		strcpy(outputName, name);
		izlaz = fopen(name, "wb");
		if (izlaz == NULL)
		{
			fclose(input);
			return OUTPUT_FILE;
		}
	}

	i = 256 + 16;

	while (i < size - 16)//Dekriptuj datoteku
	{
		fread(block, sizeof(char), 8, input);
		i += 8;
		decryptBlockDES(subkeys, block, output);
		for (j = 0; j < 8; j++)
		{
			output[j] = output[j] ^ xorblock[j];
			xorblock[j] = block[j];
		}
		fwrite(output, sizeof(char), 8, izlaz);
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys, block, output);//Dekriptuj
	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}
	i = output[7];
	fwrite(output, sizeof(char), 8 - i, izlaz);
	fclose(izlaz);//Zatvori datoteke
	fclose(input);
	return NO_ERROR;
}


DES_ERROR encrypt3DESCBC(char* inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[], unsigned char iv[])
{


	unsigned char subkeys1[16][6];//Kljucevi za runde
	unsigned char subkeys2[16][6];
	unsigned char subkeys3[16][6];
	unsigned char block[8];//Blok za enkripciju
	unsigned char btemp[8];
	unsigned char output[8];//Enkriptovan blok
	FILE* input;//Ulazni fajl
	FILE* izlaz;//Izlazni fajl
	int i = 0, j;
	int temp;
	uint32_t crc = 0;//CRC kod
	unsigned char name[256] = { 0 };//Ime ulaznog fajla

	if (!isValidDESKey(key1))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key2))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key3))
	{
		return INVALID_KEY;
	}

	output[0] = iv[0];
	output[1] = iv[1];
	output[2] = iv[2];
	output[3] = iv[3];
	output[4] = iv[4];
	output[5] = iv[5];
	output[6] = iv[6];
	output[7] = iv[7];

	if (strlen(inputName) > 255)//U Windowsu je ogranicenje 255 karaktera + NULL
	{
		return NAME_TOO_LONG;
	}

	strcpy(name, inputName);//Prekopiraj ime pre enkripciju
	crc = crc32(crc, name, 256);//Izracunaj CRC

	input = fopen(inputName, "rb");//Otvori ulaznu datoteku
	if (input == NULL)
	{
		return INPUT_FILE;//Problem sa otvaranjem ulaza
	}

	izlaz = fopen(outputName, "wb");//Otvori izlaznu datoteku
	if (izlaz == NULL)
	{
		fclose(input);
		return OUTPUT_FILE;//problem sa otvaranjem izlaza
	}

	subkeysDES(key1, subkeys1);//Izracunaj Kljuceve za razlicite runde
	subkeysDES(key2, subkeys2);
	subkeysDES(key3, subkeys3);

	while ((temp = fread(block, sizeof(char), 8, input)) != 0)//Citaj blok po blok i racunaj CRC
	{
		crc = crc32(crc, block, temp);
	}


	rewind(input);//Vrati na pocetak

	block[0] = ((unsigned int)crc >> 24);//Spremi CRC za enkripciju
	block[1] = ((unsigned int)crc >> 16);
	block[2] = ((unsigned int)crc >> 8);
	block[3] = (unsigned int)crc;


	block[4] = block[5] = block[6] = block[7] = 0;//Preostala 4 bajta su popuna

	for (j = 0; j < 8; j++)
	{
		block[j] = block[j] ^ output[j];
	}

	if (!fwrite(iv, sizeof(char), 8, izlaz))//Pisi CRC
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	encryptBlockDES(subkeys1, block, output);//Enkriptuj CRC
	decryptBlockDES(subkeys2, output, btemp);
	encryptBlockDES(subkeys3, btemp, output);
	if (!fwrite(output, sizeof(char), 8, izlaz))//Pisi CRC
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	for (int i = 0; i < 256; i += 8)//Citaj ime
	{
		for (j = 0; j < 8; j++)
		{
			block[j] = name[i + j] ^ output[j];
		}
		encryptBlockDES(subkeys1, block, output);//Enkriptuj ga
		decryptBlockDES(subkeys2, output, btemp);
		encryptBlockDES(subkeys3, btemp, output);
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	while ((temp = fread(block, sizeof(char), 8, input)) == 8)//Citaj fajl
	{
		for (j = 0; j < 8; j++)
		{
			block[j] = block[j] ^ output[j];
		}
		encryptBlockDES(subkeys1, block, output);//Enkriptuj ga
		decryptBlockDES(subkeys2, output, btemp);
		encryptBlockDES(subkeys3, btemp, output);
		if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi ga u datoteku
		{
			fclose(input);
			fclose(izlaz);
			return FILE_WRITE;//Greska pri pisanju
		}
	}

	j = 8 - temp;//Koliko treba karaktera za padding
	for (; temp < 8; temp++)//Dopuni padding-om
	{
		block[temp] = j;
	}

	for (j = 0; j < 8; j++)
	{
		block[j] = block[j] ^ output[j];
	}

	encryptBlockDES(subkeys1, block, output);//Enkriptuj
	decryptBlockDES(subkeys2, output, btemp);
	encryptBlockDES(subkeys3, btemp, output);
	if (!fwrite(output, sizeof(char), 8, izlaz)) //Pisi u datoteku
	{
		fclose(input);
		fclose(izlaz);
		return FILE_WRITE;//Greska pri pisanju
	}

	fclose(izlaz);//Zatvori izlaznu datoteku
	fclose(input);//Zatvori ulaznu datoteku
	return NO_ERROR;
}


DES_ERROR decrypt3DESCBC(char * inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[])
{
	unsigned char subkeys1[16][6];//Kljucevi za podrunde
	unsigned char subkeys2[16][6];
	unsigned char subkeys3[16][6];
	unsigned char block[8];//Blok sifrovanog teksta
	unsigned char btemp[8];
	unsigned char output[8];//Blok desifrovanog teksta
	unsigned char xorblock[8];
	unsigned char name[256];//Naziv dekriptovanog fajla
	unsigned char randomNo[3];//Slucajan broj za modifikaciju naziva
	uint32_t crc = 0;//CRC koji generisemo iz dekriptovanog fajla
	uint32_t crc_file;//CRC koji citamo iz dekriptovanog fajla
	FILE* izlaz;//Ispis
	FILE* input;//Citanje
	unsigned int i = 0, j;//Brojac
	unsigned int size;//Velicina fajla

	if (!isValidDESKey(key1))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key2))
	{
		return INVALID_KEY;
	}

	if (!isValidDESKey(key3))
	{
		return INVALID_KEY;
	}

	input = fopen(inputName, "rb");
	if (input == NULL)
		return INPUT_FILE;//Greska pri otvaranju ulaznog fajla

	if (fseek(input, 0, SEEK_END))
	{
		fclose(input);
		return SEEK;//Greska pri premotavanju
	}



	size = ftell(input) + 1;//Velicina fajla
	subkeysDES(key1, subkeys1);//Prosirivanje kljuca
	rewind(input);//Premotaj fajl

	fread(xorblock, sizeof(char), 8, input);
	fread(block, sizeof(char), 8, input);//Ucitaj CRC
	decryptBlockDES(subkeys1, block, output);//Dekriptuj CRC
	encryptBlockDES(subkeys2, output, btemp);
	decryptBlockDES(subkeys3, btemp, output);

	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}

	crc_file = output[0] << 24 | output[1] << 16 | output[2] << 8 | output[3];//Upamti CRC

	i = 16;
	while (i < size - 16)//Prodji kroz datoteku
	{
		fread(block, sizeof(char), 8, input);
		i += 8;
		decryptBlockDES(subkeys1, block, output);
		encryptBlockDES(subkeys2, output, btemp);
		decryptBlockDES(subkeys3, btemp, output);
		for (j = 0; j < 8; j++)
		{
			output[j] = output[j] ^ xorblock[j];
			xorblock[j] = block[j];
		}
		crc = crc32(crc, output, 8);//Racunaj CRC
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys1, block, output);
	encryptBlockDES(subkeys2, output, btemp);
	decryptBlockDES(subkeys3, btemp, output);
	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}
	i = output[7];
	crc = crc32(crc, output, 8 - i);//Racunaj CRC

	if (crc != crc_file)
	{
		return CRC_MISMATCH;//CRCovi se ne poklapaju
	}


	rewind(input);//Premotaj

	fread(xorblock, sizeof(char), 8, input);
	fread(block, sizeof(char), 8, input);//CRC
	decryptBlockDES(subkeys1, block, output);//Dekriptuj CRC
	encryptBlockDES(subkeys2, output, btemp);
	decryptBlockDES(subkeys3, btemp, output);

	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}

	for (i = 0; i < 256; i += 8)//Dekriptuj naziv
	{
		fread(block, sizeof(char), 8, input);
		decryptBlockDES(subkeys1, block, name + i);
		encryptBlockDES(subkeys2, name+i, btemp);
		decryptBlockDES(subkeys3, btemp, name+i);
		for (j = 0; j < 8; j++)
		{
			name[i + j] = name[i + j] ^ xorblock[j];
			xorblock[j] = block[j];
		}
	}


	izlaz = fopen(name, "rb");//Proveri postoji li file datog naziva
	if (izlaz == NULL)//Ako ne postoji, pisi
	{
		izlaz = fopen(name, "wb");
	}
	else
	{
		fclose(izlaz);//Ako postoji, dodaj na kraj slucajan dvocifren broj i pisi
		_itoa(rand() % 100, randomNo, 10);
		name[254] = '\0';
		strcat(name, randomNo);
		strcpy(outputName, name);
		izlaz = fopen(name, "wb");
	}

	i = 256 + 16;

	while (i < size - 16)//Dekriptuj datoteku
	{
		fread(block, sizeof(char), 8, input);
		i += 8;
		decryptBlockDES(subkeys1, block, output);
		encryptBlockDES(subkeys2, output, btemp);
		decryptBlockDES(subkeys3, btemp, output);
		for (j = 0; j < 8; j++)
		{
			output[j] = output[j] ^ xorblock[j];
			xorblock[j] = block[j];
		}
		fwrite(output, sizeof(char), 8, izlaz);
	}

	fread(block, sizeof(char), 8, input);//Poslednji blok


	decryptBlockDES(subkeys1, block, output);//Dekriptuj
	encryptBlockDES(subkeys2, output, btemp);
	decryptBlockDES(subkeys3, btemp, output);
	for (j = 0; j < 8; j++)
	{
		output[j] = output[j] ^ xorblock[j];
		xorblock[j] = block[j];
	}
	i = output[7];
	fwrite(output, sizeof(char), 8 - i, izlaz);
	fclose(izlaz);//Zatvori datoteke
	fclose(input);
	return NO_ERROR;
}
