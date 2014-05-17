/**
* @file queue.h
* @author Ђорђе Живановић
* @brief Садржи дефиниције листи кључева и листи имена фајлова. 
* Садржи функције за рад са листама @see
*/
#ifndef _queue_h_
#define _queue_h_
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct NodeFile NodeFile;

typedef NodeFile *NodeFilePt;

struct NodeFile{
	char *fileName; /**< Име фајла*/
	NodeFilePt next;
};

typedef struct QueueFile {
	NodeFilePt begin, end;
} QueueFile;

typedef QueueFile *QueueFilePt;

int EmptyQFile(QueueFilePt ptQ);

int PushBackFile(QueueFilePt ptQ, char *nameFile);

int PushFrontFile(QueueFilePt ptQ, char *nameFile);

char *PopBackFile(QueueFilePt ptQ);

char *PopFrontFile(QueueFilePt ptQ);



#endif