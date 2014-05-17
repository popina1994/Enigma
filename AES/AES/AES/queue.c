#include "queue.h"
#include <string.h>
#include <stdio.h>

//proverava da li je lista prazna
int EmptyQFile(QueueFilePt ptQ) {
	if (ptQ->begin == NULL)
		return 1;
	return 0;
}

// dodaje na kraj liste
int PushBackFile(QueueFilePt ptQ, char *nameFile) {
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

//dodaje na pocetak liste
int PushFrontFile(QueueFilePt ptQ, char *nameFile) {
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
}

//vraca ime prvog fajla u listi
char *First(QueueFilePt ptQ) {
	if (ptQ->begin == NULL)
		return NULL;
	return ptQ->begin->fileName;
}

//vraca ime prvog fajla i skida ga iz liste, memorija koju koristi ovaj string nije dealocirana
char *PopFrontFile(QueueFilePt ptQ) {
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

int main() {
	NodeFile node;
	return 0;
	
}
