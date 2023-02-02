/*
* Name: Brandon Xu
* Project: DigitalMusicManager
* File: header.h
* Date: 2/1/23
* Description: Header file containing library inclusions and struct and function declarations to be used throughout the project
*/
#define _CRT_SECURE_NO_WARNINGS

//library imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* STRUCT
* Name: Duration
* Description: Represents a song length in time. Contains seconds and minutes to represent this time. 
*/

typedef struct duration {
	int minutes;
	int seconds;
} Duration;

/*
* STRUCT
* Name: Record
* Description: Represents individual songs. Contains artist, album, song, and genre name, song length 
*			   (represented by struct Duration), # of times played, and a rating (1-5). 
*/
typedef struct record {
	char artist[32];
	char album[32];
	char song[32];
	char genre[32];
	Duration length;
	int timesPlayed;
	int rating;
	
} Record;

/*
* STRUCT
* Name: Node
* Description: Node that will be used in the doubly linked list. Contains a record and pointers to
*			   next and previous nodes.
*/
typedef struct node {
	Record data;
	struct Node* next;
	struct Node* prev;
} Node;

/*
* FUNCTION
* Name: handleMainMenu
* Description: Displays menu, prompts user input, and handles the input. Returns int representing the
*			   user choice.
* Inputs: void
* Outputs: returns int
*/ 
int handleMainMenu(void);

/*
* FUNCTION
* Name: makeNode
* Description: Allocates space for a node and stores a record in it
* Inputs: Record
* Outputs: returns Node*
*/
Node* makeNode(Record data);

/*
* FUNCTION
* Name: insertEnd
* Description: Inserts a node at the end of the list with the given data
* Inputs: Node** head, Record data
* Outputs: void
*/
void insertEnd(Node** head, Record data);

/*
* FUNCTION
* Name: printList
* Description: Iterates through list from the front and prints each record
* Inputs: Node* head
* Outputs: void (prints to console)
*/
void printList(Node* head);

/*
* FUNCTION
* Name: replaceComma
* Description: Helper function for parseLine function that replaces commas contained by quotations with a
*			   special character (!)
* Inputs: char line[]
* Outputs: void
*/
int replaceComma(char line[]);

/*
* FUNCTION
* Name: parseLine
* Description: Gets a line from the file, tokenizes each field, and stores them into a Record struct
* Inputs: FILE* file
* Outputs: Record
*/
Record parseLine(FILE* file);

/*
* FUNCTION
* Name: load
* Description: Reads all records from a file and creates a dynamic doubly linked list containing the 
*			   records. Returns the pointer to the list head. 
* Inputs: FILE* file
* Outputs: returns Node* head
*/
Node* load(FILE* file);

/*
* FUNCTION
* Name: store
* Description: Writes the current records in the doubly linked list into the musicPlayList.csv file.
*			   This function will completely rewrite the previous contents of the file.
* Inputs: FILE* file, Node* head
* Outputs: void
*/
void store(FILE* file, Node* head);

/*
* FUNCTION
* Name: display
* Description: Prints records to the console. This command supports two methods, one of which is selected
*			   by the user:
*				- print all records
*				- print all records that match an artist
* Inputs: Node* head
* Outputs: void (prints to console)
*/
void display(Node* head);

/*
* FUNCTION
* Name: edit
* Description: Prompts the user to find records by artist and to choose which record to edit. The user is
*			   able to edit any of the attributes in the record.
* Inputs: Node* head
* Outputs: void
*/
void edit(Node* head);