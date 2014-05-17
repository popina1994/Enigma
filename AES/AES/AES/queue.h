/**
* @file queue.h
* @author Djordje Zivanovic
*/

#ifndef _queue_h_
#define _queue_h_
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned __int32 UInt32;

typedef struct NodeFile NodeFile;

typedef NodeFile *NodeFilePt;

struct NodeFile{
	char *fileName; 
	NodeFilePt next;
};

typedef struct QueueFile {
	NodeFilePt begin, end;
} QueueFile;

typedef QueueFile *QueueFilePt;

typedef struct NodeKey NodeKey;

typedef NodeKey *NodeKeyPt;

struct NodeKey {
	UInt32 *key;
	NodeKeyPt next;
};

typedef struct QueueKey {
	NodeKeyPt begin, end;
	int keySize;
} QueueKey;

typedef QueueKey *QueueKeyPt;

/*Lista imena fajlova*/

void InitializeFileQ(QueueFilePt ptQ);

int EmptyFileQ(QueueFilePt ptQ);

int PushBackFileQ(QueueFilePt ptQ, char *nameFile);

int PushFrontFileQ(QueueFilePt ptQ, char *nameFile);

char *FirstFileQ(QueueFilePt ptQ);

char *PopFrontFileQ(QueueFilePt ptQ);

/* Lista kljuceva */

int EmptyKeyQ(QueueKeyPt ptQ);

void SetKeyQ(QueueKeyPt ptQ, int keySize);

int PushBackKeyQ(QueueKeyPt ptQ, UInt32 *key);

int PushFrontKeyQ(QueueKeyPt ptQ, UInt32 *key);

UInt32 *FirstKeyQ(QueueKeyPt ptQ);

UInt32 *PopFrontKeyQ(QueueKeyPt ptQ);

#endif