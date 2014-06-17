#ifndef DES-PUBLIC
#define DES_PUBLIC
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "des_internal.h"

enum errorsDES
{
	NO_ERROR = 0,
	INVALID_KEY = 1,
	NAME_TOO_LONG = 2,
	INPUT_FILE = 3,
	OUTPUT_FILE = 4,
	FILE_WRITE = 5,
	FILE_READ = 6,
	SEEK = 7,
	CRC_MISMATCH = 8
};

typedef enum errorsDES DES_ERROR;

uint32_t crc32(uint32_t crc, const void *buf, size_t size);

/*************************************************************************************/
/*inputName u svim funkcijama oznacava ime ulaznog fajla*/
/*outputName u funkcijama za enkripciju oznacava ime izlaznog fajla, a u funkcijama za dekripciju NIZ OD 256 KARAKTERA gde se smesta ime dekriptovanog fajla*/


/*key u svim funkcijama je niz od 8 neoznacenih karaktera, gde se nalazi kljuc u BINARNOM formatu, koji mora biti neparan (sadrzi jednobitni CRC)*/

/*0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 je validan kljuc. Svaki bajt ima neparan broj jedinica*/
/*0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x00 nije validan kljuc.*/


/*IV predstavlja NIZ OD 8 KARAKTERA koje treba SLUCAJNO generisati pred enkripciju. Ne bi smeo da se koristi vise od jedanput*/

/*SVE FUNKCIJE VRACAJU DES_ERROR TIP*/
/*TODO:Rigoroznija provera gresaka. I dalje ce vractai isti tip.*/
/*TODO:Komentari za dokumentaciju*/


/*DES ne treba koristiti, slab je*/
/*3DES ECB ne treba koristiti, previse otkriva o podacima: http://upload.wikimedia.org/wikipedia/commons/f/f0/Tux_ecb.jpg */
/*3DES CBC je siguran, ali je spor*/
/*Treba koristiti AES CBC, ali je Zivan tvrdoglav.*/

DES_ERROR encryptDESECB(char* inputName, char* outputName, unsigned char key[]);


DES_ERROR decryptDESECB(char * inputName, char* outputName, unsigned char key[]);


DES_ERROR encrypt3DESECB(char* inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[]);


DES_ERROR decrypt3DESECB(char * inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[]);


DES_ERROR encryptDESCBC(char* inputName, char* outputName, unsigned char key[], unsigned char iv[]);


DES_ERROR decryptDESCBC(char * inputName, char* outputName, unsigned char key[]);


DES_ERROR encrypt3DESCBC(char* inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[], unsigned char iv[]);


DES_ERROR decrypt3DESCBC(char * inputName, char* outputName, unsigned char key1[], unsigned char key2[], unsigned char key3[]);
#endif