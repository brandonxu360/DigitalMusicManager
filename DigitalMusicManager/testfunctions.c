/*
* Name: Brandon Xu
* Project: DigitalMusicManager
* File: testfunctions.c
* Date: 2/13/23
* Description: Function definitions for test functions, specifically insertTest, deleteTest, and shuffleTest
*/

#include "testheader.h"

void insertTest(void) {
	Node* head = NULL;

	Record newRecord = { "Perry,Katy" , "Witness", "Chained to the Rhythm", "pop", {4, 36}, -1, 6 };
	Node* newNode = malloc(sizeof(Node));
	
	if (newRecord.timesPlayed < 0)
		printf("ERROR: num played < 0\n");
	else if (newRecord.rating > 5 && newRecord.rating < 0)
		printf("ERROR: rating is out of range [0,5]\n");
	else
		printf("SUCCESS! Record looks good!\n");
	
	newNode->data = newRecord;

	Node* oldHead = head;
	newNode->next = head;
	if (oldHead != NULL) oldHead->prev = newNode;
	head = newNode;

	if (head == NULL)
		printf("ERROR: record not inserted\n");
	else
		printf("SUCCESS! record inserted successfully\n");

}

void deleteTest(void) {
	Node* head = malloc(sizeof(Node));

	Record newRecord = { "Perry,Katy" , "Witness", "Chained to the Rhythm", "pop", {4, 36}, 3, 5 };
	head->data = newRecord;

	//delete 
	Node* current = head;
	head = NULL;
	free(current);
	current = NULL;

	if (head != NULL)
		printf("ERROR: head != NULL\n");
	else if (current != NULL)
		printf("ERROR: memory not released\n");
	else
		printf("SUCESS! record deleted successfully\n");
}

void shuffleTest(void) {
	Node* list = NULL;

	//initialize test nodes and list for test point (3, 1, 2)
	TestNode* newNode1 = malloc(sizeof(Node));
	TestNode* newNode2 = malloc(sizeof(Node));
	TestNode* newNode3 = malloc(sizeof(Node));

	newNode1->num = 1;
	newNode1->next = NULL;
	newNode1->prev = NULL;

	newNode2->num = 2;
	newNode2->next = NULL;
	newNode2->prev = NULL;

	newNode3->num = 3;
	newNode3->next = NULL;
	newNode3->prev = NULL;

	insertFrontTest(&list, newNode3);
	insertFrontTest(&list, newNode2);
	insertFrontTest(&list, newNode1);

	TestNode* current1 = list, * current2 = list;
	int count = 0, index = 1;

	while (current1 != NULL) { //count number of elements in list
		count++;
		current1 = current1->next;
	}

	int order[3] = { 3,1,2 };

	
	for (int i = 0; i < count; i++) { //iterate through array and play songs according to number stored in array

		while (index < *(order + i)) { //move forward through list while target song index is higher than current index is
			current2 = current2->next;
			index++;
		}

		while (index > *(order + i)) { //move backwards through list while target song index is lower than current index is
			current2 = current2->prev;
			index--;
		}

		printf("%d, ", current2->num);
	}
	printf("\nExpected result: 3, 1, 2, ");

}

int insertFrontTest(TestNode **pList, TestNode* newNode) { //add TestNode to front of list
	TestNode* oldHead = *pList;

	newNode->next = *pList;

	if (oldHead != NULL) oldHead->prev = newNode;

	*pList = newNode;
}