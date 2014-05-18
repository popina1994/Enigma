#include "queue.h"
#include <string.h>
#include <stdio.h>

// postavlja pokazivace na prvi i poslednji element liste na NULL
void InitalizeFileQ(QueueFilePt ptQ) {
	ptQ->begin = NULL;
	ptQ->end = NULL;
}

//proverava da li je lista prazna, vraca 1 ako jeste, 0 inace
int EmptyFileQ(QueueFilePt ptQ) {
	if (ptQ->begin == NULL)
		return 1;
	return 0;
}

// dodaje ime fajla na kraj liste, 0 vraca ako nije uspelo, 1 inace
int PushBackFileQ(QueueFilePt ptQ, char *nameFile) {
	NodeFilePt newNode; //novi cvor

	newNode = (NodeFilePt)malloc(sizeof(NodeFile));
	if (newNode == NULL) // nema memorije na heap-u
		return 0;
	
	newNode->fileName = (char *)malloc(strlen(nameFile));
	if (newNode->fileName == NULL) // nema memorije na hipu
		return 0;

	strcpy(newNode->fileName, nameFile);
	newNode->next = NULL;

	if (ptQ->end == NULL) {
		ptQ->end = newNode;
		ptQ->begin = newNode;
	}
	else {
		ptQ->end->next = newNode;
		ptQ->end = ptQ->end->next;
	}
	return 1; // sve dobro proslo
}

//dodaje na ime fajla na pocetak liste, 0 ako nije uspelo, 1 inace
int PushFrontFileQ(QueueFilePt ptQ, char *nameFile) {
	NodeFilePt newNode; //novi cvor

	newNode = (NodeFilePt)malloc(sizeof(NodeFile));
	if (newNode == NULL) // nema memorije na heap-u
		return 0;

	newNode->fileName = (char *)malloc(strlen(nameFile));
	if (newNode->fileName == NULL) // nema memorije na hipu
		return 0;

	strcpy(newNode->fileName, nameFile);
	newNode->next = NULL;

	if (ptQ->end == NULL) {
		ptQ->end = newNode;
		ptQ->begin = newNode;
	}
	else {
		newNode->next = ptQ->begin;
		ptQ->begin = newNode;
	}
	return 1; // sve dobro proslo
}

//vraca ime prvog fajla u listi
char *FirstFileQ(QueueFilePt ptQ) {
	if (ptQ->begin == NULL)
		return NULL;
	return ptQ->begin->fileName;
}

//vraca ime prvog fajla i skida ga iz liste, memorija koju koristi ovaj string nije dealocirana
char *PopFrontFileQ(QueueFilePt ptQ) {
	NodeFilePt ptNode;
	char *s;

	if (ptQ->begin == NULL) {
		printf("ERROR, LISTA JE PRAZNA");
		return NULL;
	}

	ptNode = ptQ->begin; // cvor za brisanje
	s = ptNode->fileName;

	ptQ->begin = ptQ->begin->next;

	free(ptNode);
	return  s;
}

// postavlja begin i end na NULL
void InitializeKeyQ(QueueKeyPt ptQ) {
	ptQ->begin = NULL;
	ptQ->end = NULL;
}

// vraca 1 ako je lista prazna, 0 inace
int EmptyKeyQ(QueueKeyPt ptQ) {
	if (ptQ->begin == NULL)
		return 1;
	return 0;
}

// postavlja velicinu kljuca
void SetKeyQ(QueueKeyPt ptQ, int keySize) {
	ptQ->keySize = keySize;
}

// stavlja kljuc na kraj liste, vraca 1 ako je uspesno, 0 ako je neuspesno
int PushBackKeyQ(QueueKeyPt ptQ, UChar *key) {
	NodeKeyPt newNode;
	int cnt, keyByte = ptQ->keySize / 8;

	newNode = (NodeKeyPt)malloc(sizeof(NodeKey));
	if (newNode == NULL) // nema memorije na heap-u
		return 0;

	newNode->key = (UChar*)malloc(keyByte);
	if (newNode->key == NULL) // nema memorije na heap-u
		return 0;
	
	for (cnt = 0; cnt < keyByte; cnt++)
		newNode->key[cnt] = key[cnt];
	newNode->next = NULL;

	if (ptQ->end == NULL) {
		ptQ->end = newNode;
		ptQ->begin = newNode;
	}
	else {
		ptQ->end->next = newNode;
		ptQ->end = ptQ->end->next;
	}
	return 1; // sve dobro proslo
}
	
//stavlja na pocetak liste kljuceva
int PushFrontKeyQ(QueueKeyPt ptQ, UChar *key) {
	NodeKeyPt newNode; //novi cvor
	int cnt, keyByte = ptQ->keySize;

	newNode = (NodeKeyPt)malloc(sizeof(NodeKey));
	if (newNode == NULL) // nema memorije na heap-u
		return 0;

	newNode->key = (UChar*)malloc(keyByte);
	if (newNode->key == NULL) // nema memorije na hipu
		return 0;

	for (cnt = 0; cnt < keyByte; cnt++)
		newNode->key[cnt] = key[cnt];
	newNode->next = NULL;

	if (ptQ->end == NULL) {
		ptQ->end = newNode;
		ptQ->begin = newNode;
	}
	else {
		newNode->next = ptQ->begin;
		ptQ->begin = newNode;
	}
	return 1; // sve dobro proslo
}

// vraca prvi cvor iz liste kljuceva
UChar *FirstKeyQ(QueueKeyPt ptQ) {
	if (ptQ->begin == NULL)
		return NULL;
	return ptQ->begin->key;
}


//vraca prvi cvor iz liste kljuceva, memorija za kljuc nije dealocirana!
UChar *PopFrontKeyQ(QueueKeyPt ptQ) {
	NodeKeyPt ptNode;
	UChar *s;

	if (ptQ->begin == NULL) {
		printf("ERROR, LISTA JE PRAZNA");
		return NULL;
	}

	ptNode = ptQ->begin; // cvor za brisanje
	s = ptNode->key;

	ptQ->begin = ptQ->begin->next;

	free(ptNode);
	return  s;
}

int main() {

}