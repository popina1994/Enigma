#include "aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>

UInt32 UCharToUInt32(const UChar byte[]) {
	UInt32 keyPart = 0;
	keyPart |= (UInt32)byte[0] << 24; // svaka cifra se pakuje u UInt32
	keyPart |= (UInt32)byte[1] << 16;
	keyPart |= (UInt32)byte[2] << 8;
	keyPart |= (UInt32)byte[3] << 0;
	return keyPart;
}

void AddRoundKey(UChar state[][4], const UInt32 key[]) {
	UChar subKey[4];
	// subKey 1
	subKey[0] = key[0] >> 24;
	subKey[1] = key[0] >> 16;
	subKey[2] = key[0] >> 8;
	subKey[3] = key[0];
	state[0][0] ^= subKey[0];
	state[1][0] ^= subKey[1];
	state[2][0] ^= subKey[2];
	state[3][0] ^= subKey[3];

	//subKey 2
	subKey[0] = key[1] >> 24;
	subKey[1] = key[1] >> 16;
	subKey[2] = key[1] >> 8;
	subKey[3] = key[1];
	state[0][1] ^= subKey[0];
	state[1][1] ^= subKey[1];
	state[2][1] ^= subKey[2];
	state[3][1] ^= subKey[3];

	//subKey 3
	subKey[0] = key[2] >> 24;
	subKey[1] = key[2] >> 16;
	subKey[2] = key[2] >> 8;
	subKey[3] = key[2];
	state[0][2] ^= subKey[0];
	state[1][2] ^= subKey[1];
	state[2][2] ^= subKey[2];
	state[3][2] ^= subKey[3];

	//subKey 4
	subKey[0] = key[3] >> 24;
	subKey[1] = key[3] >> 16;
	subKey[2] = key[3] >> 8;
	subKey[3] = key[3];
	state[0][3] ^= subKey[0];
	state[1][3] ^= subKey[1];
	state[2][3] ^= subKey[2];
	state[3][3] ^= subKey[3];
}

void SubBytes(UChar state[][4]) {
	state[0][0] = S_BOX[state[0][0] >> 4][state[0][0] & 0x0F];
	state[0][1] = S_BOX[state[0][1] >> 4][state[0][1] & 0x0F];
	state[0][2] = S_BOX[state[0][2] >> 4][state[0][2] & 0x0F];
	state[0][3] = S_BOX[state[0][3] >> 4][state[0][3] & 0x0F];
	state[1][0] = S_BOX[state[1][0] >> 4][state[1][0] & 0x0F];
	state[1][1] = S_BOX[state[1][1] >> 4][state[1][1] & 0x0F];
	state[1][2] = S_BOX[state[1][2] >> 4][state[1][2] & 0x0F];
	state[1][3] = S_BOX[state[1][3] >> 4][state[1][3] & 0x0F];
	state[2][0] = S_BOX[state[2][0] >> 4][state[2][0] & 0x0F];
	state[2][1] = S_BOX[state[2][1] >> 4][state[2][1] & 0x0F];
	state[2][2] = S_BOX[state[2][2] >> 4][state[2][2] & 0x0F];
	state[2][3] = S_BOX[state[2][3] >> 4][state[2][3] & 0x0F];
	state[3][0] = S_BOX[state[3][0] >> 4][state[3][0] & 0x0F];
	state[3][1] = S_BOX[state[3][1] >> 4][state[3][1] & 0x0F];
	state[3][2] = S_BOX[state[3][2] >> 4][state[3][2] & 0x0F];
	state[3][3] = S_BOX[state[3][3] >> 4][state[3][3] & 0x0F];
}

void InvSubBytes(UChar state[][4]) {
	state[0][0] = INVS_BOX[state[0][0] >> 4][state[0][0] & 0x0F];
	state[0][1] = INVS_BOX[state[0][1] >> 4][state[0][1] & 0x0F];
	state[0][2] = INVS_BOX[state[0][2] >> 4][state[0][2] & 0x0F];
	state[0][3] = INVS_BOX[state[0][3] >> 4][state[0][3] & 0x0F];
	state[1][0] = INVS_BOX[state[1][0] >> 4][state[1][0] & 0x0F];
	state[1][1] = INVS_BOX[state[1][1] >> 4][state[1][1] & 0x0F];
	state[1][2] = INVS_BOX[state[1][2] >> 4][state[1][2] & 0x0F];
	state[1][3] = INVS_BOX[state[1][3] >> 4][state[1][3] & 0x0F];
	state[2][0] = INVS_BOX[state[2][0] >> 4][state[2][0] & 0x0F];
	state[2][1] = INVS_BOX[state[2][1] >> 4][state[2][1] & 0x0F];
	state[2][2] = INVS_BOX[state[2][2] >> 4][state[2][2] & 0x0F];
	state[2][3] = INVS_BOX[state[2][3] >> 4][state[2][3] & 0x0F];
	state[3][0] = INVS_BOX[state[3][0] >> 4][state[3][0] & 0x0F];
	state[3][1] = INVS_BOX[state[3][1] >> 4][state[3][1] & 0x0F];
	state[3][2] = INVS_BOX[state[3][2] >> 4][state[3][2] & 0x0F];
	state[3][3] = INVS_BOX[state[3][3] >> 4][state[3][3] & 0x0F];
}

void ShiftRows(UChar state[][4]) {
	int tmp;
	// siftuj prvu vrstu za 1 ulevo
	tmp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = tmp;
	// siftuj drugu vrstu za 2 ulevo
	tmp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = tmp;
	tmp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = tmp;
	// siftuj trecu vrstu ulevo za 3
	tmp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = tmp;
}

void InvShiftRows(UChar state[][4]) {
	int tmp;
	// Pomeri prvu vrstu udesno za 1
	tmp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = tmp;
	// Pomeri drugu vrstu udesno za 2
	tmp = state[2][3];
	state[2][3] = state[2][1];
	state[2][1] = tmp;
	tmp = state[2][2];
	state[2][2] = state[2][0];
	state[2][0] = tmp;
	// Pomeri trecu vrstu udesno za 3
	tmp = state[3][3];
	state[3][3] = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = tmp;
}

void MixColumns(UChar state[][4]) {
	UChar col[4];
	// prva kolona
	col[0] = state[0][0];
	col[1] = state[1][0];
	col[2] = state[2][0];
	col[3] = state[3][0];
	state[0][0] = G_FIELD_MUL[col[0]][0]; // mnozenje sa 2
	state[0][0] ^= G_FIELD_MUL[col[1]][1]; // mnozenje sa 3
	state[0][0] ^= col[2]; // mnozenje sa 1
	state[0][0] ^= col[3]; // mnozenje sa 1
	state[1][0] = col[0];
	state[1][0] ^= G_FIELD_MUL[col[1]][0]; // mnozenje sa 2
	state[1][0] ^= G_FIELD_MUL[col[2]][1]; // mnozenje sa 3
	state[1][0] ^= col[3];	// mnozenje sa 1
	state[2][0] = col[0];  // mnozenje sa 1
	state[2][0] ^= col[1];  // mnozenje sa 1
	state[2][0] ^= G_FIELD_MUL[col[2]][0]; // mnozenje sa 2 
	state[2][0] ^= G_FIELD_MUL[col[3]][1]; // mnozenje sa 3
	state[3][0] = G_FIELD_MUL[col[0]][1]; // mnozenje sa 3
	state[3][0] ^= col[1]; // mnozenje sa 1
	state[3][0] ^= col[2]; // mnozenje sa 1
	state[3][0] ^= G_FIELD_MUL[col[3]][0]; // mnozenje sa 2
	
	// 2 kolona
	col[0] = state[0][1];
	col[1] = state[1][1];
	col[2] = state[2][1];
	col[3] = state[3][1];
	state[0][1] = G_FIELD_MUL[col[0]][0];
	state[0][1] ^= G_FIELD_MUL[col[1]][1];
	state[0][1] ^= col[2];
	state[0][1] ^= col[3];
	state[1][1] = col[0];
	state[1][1] ^= G_FIELD_MUL[col[1]][0];
	state[1][1] ^= G_FIELD_MUL[col[2]][1];
	state[1][1] ^= col[3];
	state[2][1] = col[0];
	state[2][1] ^= col[1];
	state[2][1] ^= G_FIELD_MUL[col[2]][0];
	state[2][1] ^= G_FIELD_MUL[col[3]][1];
	state[3][1] = G_FIELD_MUL[col[0]][1];
	state[3][1] ^= col[1];
	state[3][1] ^= col[2];
	state[3][1] ^= G_FIELD_MUL[col[3]][0];
	
	// 3 kolona
	col[0] = state[0][2];
	col[1] = state[1][2];
	col[2] = state[2][2];
	col[3] = state[3][2];
	state[0][2] = G_FIELD_MUL[col[0]][0];
	state[0][2] ^= G_FIELD_MUL[col[1]][1];
	state[0][2] ^= col[2];
	state[0][2] ^= col[3];
	state[1][2] = col[0];
	state[1][2] ^= G_FIELD_MUL[col[1]][0];
	state[1][2] ^= G_FIELD_MUL[col[2]][1];
	state[1][2] ^= col[3];
	state[2][2] = col[0];
	state[2][2] ^= col[1];
	state[2][2] ^= G_FIELD_MUL[col[2]][0];
	state[2][2] ^= G_FIELD_MUL[col[3]][1];
	state[3][2] = G_FIELD_MUL[col[0]][1];
	state[3][2] ^= col[1];
	state[3][2] ^= col[2];
	state[3][2] ^= G_FIELD_MUL[col[3]][0];
	
	// 4 kolona
	col[0] = state[0][3];
	col[1] = state[1][3];
	col[2] = state[2][3];
	col[3] = state[3][3];
	state[0][3] = G_FIELD_MUL[col[0]][0];
	state[0][3] ^= G_FIELD_MUL[col[1]][1];
	state[0][3] ^= col[2];
	state[0][3] ^= col[3];
	state[1][3] = col[0];
	state[1][3] ^= G_FIELD_MUL[col[1]][0];
	state[1][3] ^= G_FIELD_MUL[col[2]][1];
	state[1][3] ^= col[3];
	state[2][3] = col[0];
	state[2][3] ^= col[1];
	state[2][3] ^= G_FIELD_MUL[col[2]][0];
	state[2][3] ^= G_FIELD_MUL[col[3]][1];
	state[3][3] = G_FIELD_MUL[col[0]][1];
	state[3][3] ^= col[1];
	state[3][3] ^= col[2];
	state[3][3] ^= G_FIELD_MUL[col[3]][0];
	}

void InvMixColumns(UChar state[][4]) {
	UChar col[4];
	// Kolona 1
	col[0] = state[0][0];
	col[1] = state[1][0];
	col[2] = state[2][0];
	col[3] = state[3][0];
	state[0][0] = G_FIELD_MUL[col[0]][5]; // mnozenje sa E u Galoaovom polju
	state[0][0] ^= G_FIELD_MUL[col[1]][3]; // mnozenje sa B u Galoavom polju
	state[0][0] ^= G_FIELD_MUL[col[2]][4]; // mnozenje sa D u Galoaovom polju
	state[0][0] ^= G_FIELD_MUL[col[3]][2]; // mnozenje sa A u Galoaovom polju
	state[1][0] = G_FIELD_MUL[col[0]][2];
	state[1][0] ^= G_FIELD_MUL[col[1]][5];
	state[1][0] ^= G_FIELD_MUL[col[2]][3];
	state[1][0] ^= G_FIELD_MUL[col[3]][4];
	state[2][0] = G_FIELD_MUL[col[0]][4];
	state[2][0] ^= G_FIELD_MUL[col[1]][2];
	state[2][0] ^= G_FIELD_MUL[col[2]][5];
	state[2][0] ^= G_FIELD_MUL[col[3]][3];
	state[3][0] = G_FIELD_MUL[col[0]][3];
	state[3][0] ^= G_FIELD_MUL[col[1]][4];
	state[3][0] ^= G_FIELD_MUL[col[2]][2];
	state[3][0] ^= G_FIELD_MUL[col[3]][5];
	// Kolona 2
	col[0] = state[0][1];
	col[1] = state[1][1];
	col[2] = state[2][1];
	col[3] = state[3][1];
	state[0][1] = G_FIELD_MUL[col[0]][5];
	state[0][1] ^= G_FIELD_MUL[col[1]][3];
	state[0][1] ^= G_FIELD_MUL[col[2]][4];
	state[0][1] ^= G_FIELD_MUL[col[3]][2];
	state[1][1] = G_FIELD_MUL[col[0]][2];
	state[1][1] ^= G_FIELD_MUL[col[1]][5];
	state[1][1] ^= G_FIELD_MUL[col[2]][3];
	state[1][1] ^= G_FIELD_MUL[col[3]][4];
	state[2][1] = G_FIELD_MUL[col[0]][4];
	state[2][1] ^= G_FIELD_MUL[col[1]][2];
	state[2][1] ^= G_FIELD_MUL[col[2]][5];
	state[2][1] ^= G_FIELD_MUL[col[3]][3];
	state[3][1] = G_FIELD_MUL[col[0]][3];
	state[3][1] ^= G_FIELD_MUL[col[1]][4];
	state[3][1] ^= G_FIELD_MUL[col[2]][2];
	state[3][1] ^= G_FIELD_MUL[col[3]][5];
	// Kolona 3
	col[0] = state[0][2];
	col[1] = state[1][2];
	col[2] = state[2][2];
	col[3] = state[3][2];
	state[0][2] = G_FIELD_MUL[col[0]][5];
	state[0][2] ^= G_FIELD_MUL[col[1]][3];
	state[0][2] ^= G_FIELD_MUL[col[2]][4];
	state[0][2] ^= G_FIELD_MUL[col[3]][2];
	state[1][2] = G_FIELD_MUL[col[0]][2];
	state[1][2] ^= G_FIELD_MUL[col[1]][5];
	state[1][2] ^= G_FIELD_MUL[col[2]][3];
	state[1][2] ^= G_FIELD_MUL[col[3]][4];
	state[2][2] = G_FIELD_MUL[col[0]][4];
	state[2][2] ^= G_FIELD_MUL[col[1]][2];
	state[2][2] ^= G_FIELD_MUL[col[2]][5];
	state[2][2] ^= G_FIELD_MUL[col[3]][3];
	state[3][2] = G_FIELD_MUL[col[0]][3];
	state[3][2] ^= G_FIELD_MUL[col[1]][4];
	state[3][2] ^= G_FIELD_MUL[col[2]][2];
	state[3][2] ^= G_FIELD_MUL[col[3]][5];
	// Kolona 4
	col[0] = state[0][3];
	col[1] = state[1][3];
	col[2] = state[2][3];
	col[3] = state[3][3];
	state[0][3] = G_FIELD_MUL[col[0]][5];
	state[0][3] ^= G_FIELD_MUL[col[1]][3];
	state[0][3] ^= G_FIELD_MUL[col[2]][4];
	state[0][3] ^= G_FIELD_MUL[col[3]][2];
	state[1][3] = G_FIELD_MUL[col[0]][2];
	state[1][3] ^= G_FIELD_MUL[col[1]][5];
	state[1][3] ^= G_FIELD_MUL[col[2]][3];
	state[1][3] ^= G_FIELD_MUL[col[3]][4];
	state[2][3] = G_FIELD_MUL[col[0]][4];
	state[2][3] ^= G_FIELD_MUL[col[1]][2];
	state[2][3] ^= G_FIELD_MUL[col[2]][5];
	state[2][3] ^= G_FIELD_MUL[col[3]][3];
	state[3][3] = G_FIELD_MUL[col[0]][3];
	state[3][3] ^= G_FIELD_MUL[col[1]][4];
	state[3][3] ^= G_FIELD_MUL[col[2]][2];
	state[3][3] ^= G_FIELD_MUL[col[3]][5];
}

void RotWord(UInt32 *wordPt) {
	*wordPt = (*wordPt >> 24) | (*wordPt << 8);
}

UInt32 SubWord(const UInt32 word) {
	UInt32 sol;
	sol = S_BOX[word << 24 >> 28][word << 28 >> 28]; // prvi bajt s leva
	sol += S_BOX[word << 16 >> 28][word << 20 >> 28] << 8; // drugi bajt s leva
	sol += S_BOX[word << 8 >> 28][word << 12 >> 28] << 16; // treci bajt s leva 
	sol += S_BOX[word >> 28][word << 4 >> 28] << 24; // cetvrti bajt s leva
	return sol;
}
// racunacu da je ceo key preveden u hex 
// keySize ispravan, provera u glavnoj funkciji
// alokacija i dealokacija za w u glavnoj funkciji
// vraca -1 ako nema memorije
// vraca 0 ako je sve dobro
// vraca 1 ako je losa duzina kljuca
int KeyExpansion(const UChar key[], UInt32 **word, const int keySize) {
	int numRounds, numCol, cnt;
	UInt32 tmp;


	switch (keySize) { // broj rundi u zavisnosti od kljuca
	case 128: numRounds = 10; numCol = 4; break;
	case 192: numRounds = 12; numCol = 6; break;
	case 256: numRounds = 14; numCol = 8; break;
	default: return 1; // nije dobra duzina kljuca
	}

	if ((*word = malloc((NUM_ROW * numRounds + NUM_ROW) * sizeof(UInt32*))) == NULL) {
		fprintf(stderr, "Nedovoljno memorije\n");
		return -1;
	}
	
	for (cnt = 0; cnt < numCol; cnt++)
		(*word)[cnt] = UCharToUInt32(key + NUM_ROW * cnt); // stvaranje reci od niza karaktera

	for (cnt = numCol; cnt < NUM_ROW * (numRounds + 1); cnt++) {
		tmp = (*word)[cnt - 1]; 
		if ((cnt % numCol) == 0) {
			RotWord(&tmp);
			tmp = SubWord(tmp) ^ ROUND_CONSTANT[(cnt - 1) / numCol];
		}
		else if (numCol > 6 && cnt % numCol == 4)
			tmp = SubWord(tmp);
		(*word)[cnt] = (*word)[cnt - numCol] ^ tmp;
	}
	return 0; // sve je dobro proslo
}

void EncryptState(const UChar in[],  UChar out[], const UInt32 key[], const int keySize) {

	UChar state[4][4];
	state[0][0] = in[0];
	state[1][0] = in[1];
	state[2][0] = in[2];
	state[3][0] = in[3];
	state[0][1] = in[4];
	state[1][1] = in[5];
	state[2][1] = in[6];
	state[3][1] = in[7];
	state[0][2] = in[8];
	state[1][2] = in[9];
	state[2][2] = in[10];
	state[3][2] = in[11];
	state[0][3] = in[12];
	state[1][3] = in[13];
	state[2][3] = in[14];
	state[3][3] = in[15];

	AddRoundKey(state, &key[0]);
	SubBytes(state);
	ShiftRows(state); 
	MixColumns(state); 
	AddRoundKey(state, &key[4]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[8]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[12]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[16]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[20]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[24]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[28]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[32]);
	SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[36]);
	if (keySize > 128) {
		SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[40]);
		SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[44]);
		if (keySize > 192) {
			SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[48]);
			SubBytes(state); ShiftRows(state); MixColumns(state); AddRoundKey(state, &key[52]);
			SubBytes(state); ShiftRows(state); AddRoundKey(state, &key[56]);
		}
		else {
			SubBytes(state); ShiftRows(state); AddRoundKey(state, &key[48]);
		}
	}
	else {
		SubBytes(state); ShiftRows(state); AddRoundKey(state, &key[40]);
	}

	out[0] = state[0][0];
	out[1] = state[1][0];
	out[2] = state[2][0];
	out[3] = state[3][0];
	out[4] = state[0][1];
	out[5] = state[1][1];
	out[6] = state[2][1];
	out[7] = state[3][1];
	out[8] = state[0][2];
	out[9] = state[1][2];
	out[10] = state[2][2];
	out[11] = state[3][2];
	out[12] = state[0][3];
	out[13] = state[1][3];
	out[14] = state[2][3];
	out[15] = state[3][3];
}

void DecryptState(const UChar in[], UChar out[], const UInt32 key[], const int keySize) {
	UChar state[4][4];

	state[0][0] = in[0];
	state[1][0] = in[1];
	state[2][0] = in[2];
	state[3][0] = in[3];
	state[0][1] = in[4];
	state[1][1] = in[5];
	state[2][1] = in[6];
	state[3][1] = in[7];
	state[0][2] = in[8];
	state[1][2] = in[9];
	state[2][2] = in[10];
	state[3][2] = in[11];
	state[0][3] = in[12];
	state[1][3] = in[13];
	state[2][3] = in[14];
	state[3][3] = in[15];

	if (keySize > 128) {
		if (keySize > 192) {
			AddRoundKey(state, &key[56]);
			InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[52]); InvMixColumns(state);
			InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[48]); InvMixColumns(state);
		}
		else {
			AddRoundKey(state, &key[48]);
		}
		InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[44]); InvMixColumns(state);
		InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[40]); InvMixColumns(state);
	}
	else {
		AddRoundKey(state, &key[40]);
	}
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[36]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[32]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[28]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[24]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[20]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[16]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[12]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[8]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[4]); InvMixColumns(state);
	InvShiftRows(state); InvSubBytes(state); AddRoundKey(state, &key[0]);

	out[0] = state[0][0];
	out[1] = state[1][0];
	out[2] = state[2][0];
	out[3] = state[3][0];
	out[4] = state[0][1];
	out[5] = state[1][1];
	out[6] = state[2][1];
	out[7] = state[3][1];
	out[8] = state[0][2];
	out[9] = state[1][2];
	out[10] = state[2][2];
	out[11] = state[3][2];
	out[12] = state[0][3];
	out[13] = state[1][3];
	out[14] = state[2][3];
	out[15] = state[3][3];
}

// vraca velicinu datoteke u bajtovima
UInt32 SizeOfFile(const UChar *name) {
	UInt32 error = 1, cnt;
	UChar c;
	FILE *f;

	f = fopen(name, "rb+");
	if (f == NULL)
		return 0;

	for (cnt = 0; error == 1; cnt++) {
		fseek(f, cnt, SEEK_SET);
		error = fread(&c, sizeof(UChar), 1, f); 
	}
	return cnt - 1;
}

// vraca 1 ako je dobro
// vraca 0 ako nije dobro
int ReadData(FILE *f,  UChar state[], UInt32 *sizeToEnd) {
	int cnt;

	memset(state, 0, STATE_SIZE);
	for (cnt = 0; cnt < STATE_SIZE; cnt++) {
		if (*sizeToEnd == 0) // broji broj procesiranih bajtova
			return 1;
		(*sizeToEnd)--;
		if (fread(state + cnt, sizeof(UChar), 1, f) != 1) { // citaj jedan bajt
			fclose(f);
			return 0;
		}
	}
	return 1;
}

// vraca 1 ako je sve dobro proslo
// vraca 0 ako nije dobro
int WriteData(FILE *f, const UChar state[]) {
	if (fwrite(state, STATE_SIZE, 1, f) == 1)
		return 1;
	return 0;
}

// prevodi 4 bajta u state
void UInt32ToState(const UInt32 num, UChar state[]) {
	memset(state, 0, STATE_SIZE);
	state[STATE_SIZE - 1] = num << 24 >> 24;
	state[STATE_SIZE - 2] = num << 16 >> 24;
	state[STATE_SIZE - 3] = num << 8 >> 24;
	state[STATE_SIZE - 4] = num >> 24;
}

UInt32 StateToUInt32(const UChar state[]) {
	UInt32 sol = 0;
	sol = state[STATE_SIZE - 1];
	sol |= state[STATE_SIZE - 2] << 8;
	sol |= state[STATE_SIZE - 3] << 16;
	sol |= state[STATE_SIZE - 4] << 24;
	return sol;
}

// vraca string ako je uspelo stvaranje imena izlazne datoteke
// vraca NULL ako nije uspelo
UChar *CypherFileName(const UChar *inFileName) {
	UChar *outFileName;
	UChar full[MAX_FILE_PATH ]; // maksimalna duzina putanje
	UInt32 maxFileLen; // maksimalna duzina imena

	if (_fullpath(full, inFileName, MAX_FILE_PATH) == NULL) { return NULL; } // proverava duzinu putanje
	
	maxFileLen = MAX_FILE_PATH - (strlen(full) - strlen(inFileName)); // ukljuven NULL u duzinu

	outFileName = malloc((strlen(inFileName) + strlen(CRYPT_TEXT) + 1) * sizeof(UChar));  // alociora prostor
	if (outFileName == NULL) { fprintf(stderr, "Nedovoljno memorije\n"); return NULL; }


	strcpy(outFileName, inFileName);
	if (strlen(outFileName) + (strlen(CRYPT_TEXT)) + 1 > maxFileLen) // ako je duzina sifrovanog fajla + ekstenzija veca od 260
		strcpy(outFileName + maxFileLen - strlen(CRYPT_TEXT) - 1, CRYPT_TEXT);
	else strcat(outFileName, CRYPT_TEXT); // ektenzija
	
	return outFileName;
}

// vraca 0 ako je sve proteklo dobro, pri cemu *inFileName je ime
// vraca 1 ako nema dovoljno memorije
// vraca 2 ako je losa duzina imena (veca od 255 karaktera)
int DeCypherFileName(UChar **inFileName) {
	FILE *in;
	UChar full[MAX_FILE_PATH];
	UInt32 len = strlen(*inFileName),
		maxFileLen,
		pos = 0; // oznacava na koju poziciju se upisuje nasumican broj

	srand((UInt32)time(0));
	if (_fullpath(full, *inFileName, MAX_FILE_PATH) == NULL) { return 2; }
	maxFileLen = MAX_FILE_PATH - (strlen(full) - len); // sa NULL

	while ((in = fopen(*inFileName, "rb")) != NULL) {
		fclose(in); // zatvori dobro otvorenu datoteku
		
		
		if (len + 1 < maxFileLen) {
			len++; // velicina novog imena
			*inFileName = realloc(*inFileName, (len + 1) * sizeof(UChar));
			if (*inFileName == NULL) { fprintf(stderr, "Nedovoljno memorije\n"); return 1; }
		}
		

		memmove(*inFileName + 1 + pos, *inFileName, strlen(*inFileName) + 1 - pos); // treba ispitati
		
		(*inFileName)[pos] = (UChar)((DIGIT_NUM - 1)* (double)rand() / RAND_MAX + '0');
		if (len + 1 == maxFileLen) {
			(*inFileName)[len] = '\0';
			pos++;
		}
	}
	
	return 0;
}

// vraca CRC-ovano stanje
UInt32 CRC32(UInt32 crc, const  UChar state[]) {
	int  cnt;
	crc = crc ^ ~0U;

	for (cnt = 0; cnt < STATE_SIZE; cnt++)
	
		crc = CRC32_TABLE[(crc ^ state[cnt]) & 0xFF] ^ (crc >> 8);
	return crc ^ ~0U;
}

// vraca 0 ako je sve dobro
// vraca 1 ako su razliciti
int CRCCheck(FILE *in, const UInt32 key[], const int keySize) {
	UInt32 sizeOfInputFile = STATE_SIZE * CHAR_BIT; // prvi blok
	UInt32 bitLen, cnt, crc = 0, crcOriginal;
	UChar state[STATE_SIZE], outState[STATE_SIZE];

	// crc
	if (!ReadData(in, state, &sizeOfInputFile))  return 1; 
	DecryptState(state, outState, key, keySize); // bitLen
	crcOriginal = StateToUInt32(outState); // originalni CRC

	// Duzina imena
	if (!ReadData(in, state, &sizeOfInputFile)) { return 1; }
	DecryptState(state, outState, key, keySize); // bitLen
	crc = CRC32(crc, outState);
	bitLen = StateToUInt32(outState);
	sizeOfInputFile += STATE_SIZE * CHAR_BIT * (bitLen + 2); // i fajl i velicina datoteke

	//prvi deo imena
	for (cnt = 0; cnt < bitLen; cnt++) {
		if (!ReadData(in, state, &sizeOfInputFile))  return 1; 
		DecryptState(state, outState, key, keySize);
		crc = CRC32(crc, outState);
	}

	// drugi deo imena
	if (!ReadData(in, state, &sizeOfInputFile))  return 1; 
	DecryptState(state, outState, key, keySize);
	crc = CRC32(crc, outState);
	
	// velicina fajla
	if (!ReadData(in, state, &sizeOfInputFile))  return 1; 
	DecryptState(state, outState, key, keySize);
	crc = CRC32(crc, outState);
	sizeOfInputFile = StateToUInt32(outState);

	// tekst
	while (sizeOfInputFile != 0) {
		if (sizeOfInputFile < 16) {
			int tmpSize = 16; // ako korisnik nije unosi blokove po 128
			if (!ReadData(in, state, &tmpSize))
				return 1;
			DecryptState(state, outState, key, keySize);
			crc = CRC32(crc, outState);
			sizeOfInputFile = 0;
		}
		else {
			if (!ReadData(in, state, &sizeOfInputFile))  return 1; 
			DecryptState(state, outState, key, keySize);
			crc = CRC32(crc, outState);
		}
	}
	
	if (crc == crcOriginal)
		return 0; // nije menjan je sifrovani fajl
	return 1; // menjan je sifrovani fajl
}

// vraca 0 ako je sve dobro proslo
// vraca 1 ako je greska pri citanju datoteke
// vraca 2 ako je doslo do greske pri pisanju datoteke
// vraca 3 ako nema dovoljno memorije
// vraca 4 ako duzina imena nije dobra
// vraca 5 ako duzina kljuca nije 
// warning duzina imena je 255 (najvise) (ne ukljucujem NULL)
// warning Slavko mora da mi prosledi string koji je alociran na heap
// warning nemoj da strpsas fajl velicine vece od 2^32 -1, God Save us then
// vrsi padding
int CypherAES(UChar **inFileName, const UChar keyIn[], const int keySize) {
	FILE *in, *out; // ulazna i izlazna datoteka
	UChar *outFileName;
	UChar state[STATE_SIZE], outState[STATE_SIZE]; // blok koji treba sifrovati
	UInt32 sizeOfInputFile; // velicina fajla koji se sifruje
	int errorExpansion; // greska pri razvijanju kljuca
	UInt32 *key; // niz reci napravljenih od kljuca
	UInt32 bitLen; // oznacava da li je ime duze od 127, ako jeste onda je 1
	UInt32 cnt; // brojac
	UInt32 crc = 0; // crc stanje
	
	//ekspanzija kljuca
	errorExpansion = KeyExpansion(keyIn, &key, keySize);
	if (errorExpansion == -1) { fprintf(stderr, "Nema memorije\n"); return 3; }
	if (errorExpansion == 1) { fprintf(stderr, "Losa duzina kljuca\n"); return 5; }


	sizeOfInputFile = SizeOfFile(*inFileName); // da ne bi pocelo citanje

	in = fopen(*inFileName, "rb"); // citanje iz binarne
	if (in == NULL) { fprintf(stderr, "Greska pri citanju datoteke\n");return 1; }

	outFileName = CypherFileName(*inFileName);// ime stringa sa kojim se radi

	if (outFileName == NULL) { fprintf(stderr, "Nema dovoljno memorije\n");  return 3; }
	
	out = fopen(outFileName, "wb"); // otvaranje datoteke u koju pisemo
	if (out == NULL) { fprintf(stderr, "Greska pri upisivanju datoteke\n"); return 2; }

	bitLen = (strlen(*inFileName) + 1) / STATE_SIZE;

	// pisanje CRC-a
	if (!WriteData(out, outState)){ fprintf(stderr, "Greska pri upisvanju u datoteku\n"); return 2; }
	
	// pisanje bitLen
	UInt32ToState(bitLen, state); 
	crc = CRC32(crc, state);
	EncryptState(state, outState, key, keySize);
	if (!WriteData(out, outState)){ fprintf(stderr, "Greska pri upisvanju u datoteku\n"); return 2; }

	// blokova imena
	for (cnt = 0; cnt <= bitLen; cnt++) {
		strncpy(state, *inFileName + STATE_SIZE * cnt, STATE_SIZE);
		crc = CRC32(crc, state);
		EncryptState(state, outState, key, keySize);
		if (!WriteData(out, outState)){ fprintf(stderr, "Greska pri upisvanju u datoteku\n"); return 2; }
	}
	
	// pisanje velicine datoteke
	UInt32ToState(sizeOfInputFile, state);
	crc = CRC32(crc, state);
	EncryptState(state, outState, key, keySize);
	if (!WriteData(out, outState)){ fprintf(stderr, "Greska pri upisvanju u datoteku\n"); return 2; }
	
	// sifrovanje teksta
	while (sizeOfInputFile != 0) {
		if (!ReadData(in, state, &sizeOfInputFile)){ fprintf(stderr, "Greska pri citanju datoteke\n"); return 1; }
		crc = CRC32(crc, state);
		EncryptState(state, outState, key, keySize);
		if (!WriteData(out, outState)){ fprintf(stderr, "Greska pri upisvanju u datoteku\n"); return 2; }
	}

	// sifrovanje CRC-a
	rewind(out);
	UInt32ToState(crc, state);
	EncryptState(state, outState, key, keySize);
	if (!WriteData(out, outState)){ fprintf(stderr, "Greska pri upisvanju u datoteku\n"); return 2; }
	
	fclose(in); fclose(out);
	free(key);
	
	free(*inFileName);
	*inFileName = outFileName;

	return 0;
}


// vraca 0 ako je sve dobro proslo
// vraca 1 ako je greska pri citanju datoteke
// vraca 2 ako je doslo do greske pri pisanju datoteke
// vraca 3 ako nema dovoljno memorije
// vraca 4 ako duzina imena nije dobra
// vraca 5 ako duzina kljuca nije 
// vraca 6 ako nije dobro sifrovana datoteka
// warning duzina imena je 255 karaktera (najvise) (ne ukljucujem NULL)
// warning Slavko mora da mi prosledi string koji je alociran na heap
// warning nemoj da strpsas fajl velicine vece od 2^32 -1, God Save us then
// vrsi padding
int DecypherAES(UChar **inFileName, const UChar keyIn[], const int keySize) {
	FILE *in, *out; // ulazna i izlazna datoteka
	UChar *outFileName = NULL;
	UChar state[STATE_SIZE], outState[STATE_SIZE]; // blok koji treba sifrovati
	UInt32 sizeOfInputFile; // velicina fajla koji se sifruje
	int errorExpansion; // greska pri razvijanju kljuca
	int	errorFile; // Greska oko imena datoteke
	int errorCRC; // Greska oko sifrovanog fajla
	UInt32 *key; // niz reci napravljenih od kljuca
	UInt32 bitLen; // oznacava da li je duzina imena veca od 127
	UInt32 cnt; // brojac

	// razvijanje kljuca
	errorExpansion = KeyExpansion(keyIn, &key, keySize);
	if (errorExpansion == -1) { fprintf(stderr, "Nema memorije\n"); return 3; }
	if (errorExpansion == 1) { fprintf(stderr, "Losa duzina kljuca\n"); return 5; }

	
	in = fopen(*inFileName, "rb"); // citanje iz binarne
	if (in == NULL) { fprintf(stderr, "Greska pri citanju datoteke\n"); return 1; }

	sizeOfInputFile = STATE_SIZE * CHAR_BIT; // prvi blok

	// provera sifrovanja
	errorCRC = CRCCheck(in, key, keySize);
	if (errorCRC == 1) { fprintf(stderr, "Nije dobro sifrovana datoteka\n"); return 6; }
	if (errorCRC == 2) { fprintf(stderr, "Greska pri citanju datoteke\n"); return 1; }
	
	fseek(in, STATE_SIZE, SEEK_SET);
	
	// Duzina imena
	if (!ReadData(in, state, &sizeOfInputFile)) { fprintf(stderr, "Greska pri citanju datoteke\n"); return 1; }
	DecryptState(state, outState, key, keySize); // bitLen
	bitLen = StateToUInt32(outState);
	sizeOfInputFile += STATE_SIZE * CHAR_BIT * (bitLen + 2); // i fajl i velicina datoteke

	//ime
	for (cnt = 0; cnt <= bitLen; cnt++) {
		outFileName = realloc(outFileName, (cnt + 1) * STATE_SIZE);
		if (outFileName == NULL) { fprintf(stderr, "Nedovoljno memorije\n"); return 3; }
		
		if (!ReadData(in, state, &sizeOfInputFile)) { fprintf(stderr, "Greska pri citanju datoteke\n"); return 1; }
		DecryptState(state, outState, key, keySize);
		strncpy(outFileName + cnt * STATE_SIZE, outState , STATE_SIZE);
	}

	// da li moze da se napravi originalni fajl
	errorFile = DeCypherFileName(&outFileName);
	if (errorFile == 1) { fprintf(stderr, "Nedovoljno memorije\n"); return 3; }
	if (errorFile == 2) { fprintf(stderr, "Nije dobra duzina imena");  return 4; }


	// otvaranje fajla gde cemo desifrovani rezultat smesititi
	out = fopen(outFileName, "wb");
	if (out == NULL) { fprintf(stderr, "Greska pri pisanju datoteke\n"); return 2; }
	
	// velicina fajla
	if (!ReadData(in, state, &sizeOfInputFile)) { fprintf(stderr, "Greska pri citanju datoteke\n"); return 1; }
	DecryptState(state, outState, key, keySize);
	sizeOfInputFile = StateToUInt32(outState);
	
	// tekst
	while (sizeOfInputFile != 0) {
		if (sizeOfInputFile < STATE_SIZE) {
			int tmpSize = 16; // ako korisnik nije unosi blokove po 128
			if (!ReadData(in, state, &tmpSize))
				return 1;
			DecryptState(state, outState, key, keySize);
			if (fwrite(outState, sizeOfInputFile, 1, out) != 1) {
				fprintf(stderr, "Greska pri upisu\n");
				return 2;
			}
			sizeOfInputFile = 0;
		}
		else {
			if (!ReadData(in, state, &sizeOfInputFile)) { fprintf(stderr, "Greska pri citanju datoteke\n"); return 1; }
			DecryptState(state, outState, key, keySize);
			if (!WriteData(out, outState)) {fprintf(stderr, "Greska pri upisvanju u datoteku\n"); return 2;}
		}
	}

	fclose(in); fclose(out);
	free(key);

	free(*inFileName);
	*inFileName = outFileName;

	return 0;

}

int main() {
	UChar *inputName, key[] = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6\xab\xf7\x15\x88\x09\xcf\x4f\x3c";

	freopen("ulazIme.in", "r", stdin);
	inputName = (UChar*)malloc(256);
	scanf("%s", inputName);
	CypherAES(&inputName, key, 128);
	DecypherAES(&inputName, key, strlen(key) * 8);
	//DeCypherFileName(&inputName);
	system("pause");
	return 0;
}