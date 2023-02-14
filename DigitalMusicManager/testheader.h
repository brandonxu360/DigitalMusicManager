/*
* Name: Brandon Xu
* Project: DigitalMusicManager
* File: testfunctions.c
* Date: 2/13/23
* Description: Function and struct declarations for test structs and test functions,
*			   specifically TestNode and insertTest, deleteTest, and shuffleTest
*/


#pragma once

#include "header.h"

typedef struct testnode {
	int num;
	struct testnode* next;
	struct testnode* prev;
}TestNode;

void insertTest(void);
void deleteTest(void);
void shuffleTest(void);