#ifndef DES_INTERNAL
#define DES_INTERNAL

int isValidDESKey(unsigned char key[]);

void PC1DES(unsigned char input[], unsigned char key[]);

void cirShift1(unsigned char key[], unsigned char newKey[]);

void cirShift2(unsigned char key[], unsigned char newKey[]);

void PC2DES(unsigned char input[], unsigned char output[]);

void expandDES(unsigned char block[], unsigned char expBlock[]);

void permutationDES(unsigned char block[], unsigned char permuted[]);

void feistelDES(unsigned char key[], unsigned char block[], unsigned char output[]);

void IPDES(unsigned char block[], unsigned char newBlock[]);

void FPDES(unsigned char block[], unsigned char newBlock[]);

void subkeysDES(unsigned char key[], unsigned char subkeys[16][6]);

void encryptBlockDES(unsigned char subkeys[16][6], unsigned char block[], unsigned char output[]);

void decryptBlockDES(unsigned char subkeys[16][6], unsigned char block[], unsigned char output[]);

#endif