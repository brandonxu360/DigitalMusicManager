/*
* Name: Brandon Xu
* Project: DigitalMusicManager
* File: functions.c
* Date: 2/1/23
* Description: Functions file that contains the full function definitions that will be used in the main file
*/

#include "header.h"

int handleMainMenu(void) {
	int choice = 0;

	//print menu
	printf("MENU\n");
	printf("-------------------\n");
	printf("(1)   load\n");
	printf("(2)   store\n");
	printf("(3)   display\n");
	printf("(4)   insert\n");
	printf("(5)   delete\n");
	printf("(6)   edit\n");
	printf("(7)   sort\n");
	printf("(8)   rate\n");
	printf("(9)   play\n");
	printf("(10)  shuffle\n");
	printf("(11)  exit\n\n");

	//prompt user input
	printf("Input: ");

	//get user input
	scanf("%d", &choice);

	//invalid input catch
	while (choice < 1 || choice > 11) {
		printf("\n\nInvalid input detected. Try again.\n");
		printf("Input: ");
		
		scanf("%d", &choice);
	}

	return choice;
}

Node* makeNode(Record data) {
	//allocate space for a node (Record + 2 pointers to nodes)
	Node* ptr = malloc(sizeof(Node));

	//copy data into node
	ptr->data = data;

	//initialize pointers to next and previous nodes
	ptr->next = NULL;
	ptr->prev = NULL;
	
	//return pointer to node
	return ptr;
}

void insertEnd(Node** head, Record data) {
	//make new node with data and assign to ptr
	Node* ptr = makeNode(data);

	if (*head == NULL) { //if list is empty
		//set head to the new node ptr
		*head = ptr;
	}

	else { //list is not empty
		//current will be used to iterate to the end of the list
		Node* current = *head;

		while (current->next != NULL) { //iterate through the list until current = last node
			current = current->next;
		}

		//set last node's next to point to the new node ptr
		current->next = ptr;

		//set new node's prev to point to the last node
		ptr->prev = current;
	}
}

void printList(Node* head) {
	Node* current = head;
	int count = 1;

	//iterate through list until end of list is reached
	while (current != NULL) {
		printf("%d)     Song:         %s\n", count, current->data.song);
		printf("       Album:        %s\n", current->data.album);
		printf("       Artist:       %s\n", current->data.artist);
		printf("       Genre:        %s\n", current->data.genre);
		printf("       Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
		printf("       Rating:       %d\n", current->data.rating);
		printf("       Times played: %d\n\n", current->data.timesPlayed);
		current = current->next;
		
		count++;
	}
}

int replaceComma(char line[]) {
	int index = 1;//start after the first quote character

	while (line[index] != '"') { //keep iterating until the second quote character is found

		if (line[index] == ',') { // if a comma is found, replace with special character !
			line[index] = '!'; 

			return index; //return index where comma was found
		}

		index++;
	}
}

Record parseLine(FILE* file) {
	Record data;
	char line[128], *token;
	int specialCharIndex;
	 
	//get a line, return the data struct with rating = 0 if EOF (this will be detected in load function)
	data.rating = 0;
	if (!fgets(line, 127, file)) return data;


	//gets artist name
	if (line[0] == '"') { //if the name is formatted with quotation marks and a comma
		specialCharIndex = replaceComma(line); //temporarily replace comma with special character

		token = strtok(line, ","); //tokenize the name

		token[specialCharIndex] = ',';

		strcpy(data.artist, token);
	}

	else { //name is not formatted using quotation marks and comma
		token = strtok(line, ",");
		strcpy(data.artist, token);
	}
	
	//gets album name
	token = strtok(NULL, ",");
	strcpy(data.album, token);

	//gets genre
	token = strtok(NULL, ",");
	strcpy(data.song, token);

	//gets song name
	token = strtok(NULL, ",");
	strcpy(data.genre, token);

	//gets minutes
	token = strtok(NULL, ":");
	data.length.minutes = atoi(token);

	//gets seconds
	token = strtok(NULL, ",");
	data.length.seconds = atoi(token);

	//gets times played
	token = strtok(NULL, ",");
	data.timesPlayed = atoi(token);

	//gets rating played
	token = strtok(NULL, ",");
	data.rating = atoi(token);

	return data;
}

Node* load(FILE* file) {
	Record data;

	//get first line
	data = parseLine(file);

	//if first line is empty, return empty list
	if (data.rating == 0) return NULL;

	//if first line is not empty
	else {
		
		//make new head using first line 
		Node* head = makeNode(data);

		//start loop for next nodes by getting next line
		data = parseLine(file);

		while (data.rating != 0) { //parse until the end of the file

			insertEnd(&head, data); //insert nodes with the records at the end of the list

			data = parseLine(file);
			
		}

		return head;
	}

	
}

void store(FILE* file, Node* head) {
	Node* current = head;

	while (current != NULL) {
		//write record to file
		fprintf(file, "%s,%s,%s,%s,%d:%d,%d,%d\n", current->data.artist, current->data.album,
			current->data.song, current->data.genre, current->data.length.minutes,
			current->data.length.seconds, current->data.timesPlayed, current->data.rating);

		//set current to next node
		current = current->next;
	}
}

void display(Node* head) {
	int choice;

	//prompt user input
	printf("(1)   display all records\n");
	printf("(2)   search artist\n\n");
	printf("Input: ");

	//get user input
	scanf("%d", &choice);

	//invalid input catch
	while (choice < 1 || choice > 2) {
		printf("\n\nInvalid input detected. Try again.\n");
		printf("Input: ");

		scanf("%d", &choice);
	}

	system("cls");

	//choice of 1 => print all records
	if (choice == 1) printList(head);

	//choice of 2 => search for artist
	else if (choice == 2) {
		//current will be used to iterate through the list
		Node* current = head;
		char search[64];
		int count = 1; //used to number the records that are printed
		int found = 0; //indicator for if no records matching the artist are found

		//prompt artist name
		printf("Enter the name of the artist you would like to search\n\n");
		printf("Input: ");

		scanf(" %[^\n]s", search); //get input including space

		printf("\n"); //line spacer

		while (current != NULL) { //iterate until the end of the list

			if (strcmp(search, current->data.artist) == 0) { //print record if the artist names match
				printf("%d)     Song:         %s\n", count, current->data.song);
				printf("       Album:        %s\n", current->data.album);
				printf("       Artist:       %s\n", current->data.artist);
				printf("       Genre:        %s\n", current->data.genre);
				printf("       Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
				printf("       Rating:       %d\n", current->data.rating);
				printf("       Times played: %d\n\n", current->data.timesPlayed);
				count++;
				found++;
			}

			current = current->next;
		}

		if (found == 0) {
			printf("Sorry, no records matching this artist were found.\n");
		}
	}
}

void edit(Node* head) {
	Node* current = head;
	int index = 0; //important to use to iterate back to the record the user chooses if multiple records are found
	int found = 0, choice = 0, attribute = 0;
	char artist[64];

	//prompt artist name
	printf("\nEnter the name of the artist you would like to search\n\n");
	printf("Input: ");

	scanf(" %[^\n]s", artist); //get input including space

	printf("\nRESULTS");
	printf("\n--------------------------\n");

	while (current != NULL) { //iterate until the end of the list

		if (strcmp(artist, current->data.artist) == 0) { //print record with corresponding index if the artist names match
			printf("%d)     Song:         %s\n", index, current->data.song);
			printf("       Album:        %s\n", current->data.album);
			printf("       Artist:       %s\n", current->data.artist);
			printf("       Genre:        %s\n", current->data.genre);
			printf("       Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
			printf("       Rating:       %d\n", current->data.rating);
			printf("       Times played: %d\n\n", current->data.timesPlayed);

			found++;
		}
		
		index++;
		current = current->next; //move to next node
	
	}

	if (found == 0) { //no artist matching the search name is found
		printf("\nSorry, no records matching this artist were found.\n");
	}

	else {

		//prompt selection of record to modify (out of the records that match the inputted artist name)
		printf("\nEnter the number that corresponds with the record you would like to edit\n\n");
		printf("Input: ");

		scanf("%d", &choice);

		//reset current to head to iterate through list again
		current = head;

		for (int i = 0; i < choice; i++) { //iterate through the linked list until at chosen record (by index)
			current = current->next;
		}

		system("cls");

		//display record selected for modification
		printf("MODIFYING RECORD:\n\n");
		printf("Song:         %s\n", current->data.song);
		printf("Album:        %s\n", current->data.album);
		printf("Artist:       %s\n", current->data.artist);
		printf("Genre:        %s\n", current->data.genre);
		printf("Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
		printf("Rating:       %d\n", current->data.rating);
		printf("Times played: %d\n\n", current->data.timesPlayed);

		//prompt attribute selection
		printf("\nEnter the number corresponding with the attribute to edit\n");
		printf("(1)   artist\n");
		printf("(2)   album\n");
		printf("(3)   song\n");
		printf("(4)   genre\n");
		printf("(5)   length\n");
		printf("(6)   times played\n");
		printf("(7)   rating\n\n");
		printf("Input: ");

		scanf("%d", &attribute);

		printf("\n\n");

		switch (attribute) { //edit selected attribute based on user input (attribute variable)
			
			case 1: //artist
				printf("Enter the new artist name\n\n");
				printf("Input: ");
				scanf(" %[^\n]s", current->data.artist);
				break;

			case 2: //album
				printf("Enter the new album name\n\n");
				printf("Input: ");
				scanf(" %[^\n]s", current->data.album);
				break;

			case 3: //song
				printf("Enter the new song name\n\n");
				printf("Input: ");
				scanf(" %[^\n]s", current->data.song);
				break;

			case 4: //genre
				printf("Enter the new genre name\n\n");
				printf("Input: ");
				scanf(" %[^\n]s", current->data.genre);
				break;

			case 5: //length (duration)
				printf("Enter minutes: \n\n");
				printf("Input: ");
				scanf("%d", &current->data.length.minutes);

				printf("\n\nEnter seconds: \n\n");
				printf("Input: ");
				scanf("%d", &current->data.length.seconds);
				break;

			case 6: //times played 
				printf("Enter new number of times played: \n\n");
				printf("Input: ");
				scanf("%d", &current->data.timesPlayed);
				break;

			case 7: //rating
				printf("Enter new rating: \n\n");
				printf("Input: ");
				scanf("%d", &current->data.rating);
				break;
		}

		//display modified record
		printf("\nMODIFIED RECORD\n");
		printf("Song:         %s\n", current->data.song);
		printf("Album:        %s\n", current->data.album);
		printf("Artist:       %s\n", current->data.artist);
		printf("Genre:        %s\n", current->data.genre);
		printf("Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
		printf("Rating:       %d\n", current->data.rating);
		printf("Times played: %d\n\n", current->data.timesPlayed);
	}

}

void rate(Node* head) {
	Node* current = head;
	int index = 0; //important to use to iterate back to the record the user chooses if multiple records are found
	int found = 0, choice = 0, attribute = 0;
	char song[64];

	//prompt artist name
	printf("\nEnter the name of the song you would like to rate\n\n");
	printf("Input: ");

	scanf(" %[^\n]s", song); //get input including space

	printf("\nRESULTS");
	printf("\n--------------------------\n");

	while (current != NULL) { //iterate until the end of the list

		if (strcmp(song, current->data.song) == 0) { //print record with corresponding index if the song names match
			printf("%d)     Song:         %s\n", index, current->data.song);
			printf("       Album:        %s\n", current->data.album);
			printf("       Artist:       %s\n", current->data.artist);
			printf("       Genre:        %s\n", current->data.genre);
			printf("       Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
			printf("       Rating:       %d\n", current->data.rating);
			printf("       Times played: %d\n\n", current->data.timesPlayed);

			found++;
		}

		index++;
		current = current->next; //move to next node

	}

	if (found == 0) { //no artist matching the search name is found
		printf("\nSorry, no records matching this song were found.\n");
	}

	else {
		int newRating = 0;

		//prompt selection of record to modify (out of the records that match the inputted song name)
		printf("\nEnter the number that corresponds with the record you would like to edit\n\n");
		printf("Input: ");

		scanf("%d", &choice);

		//reset current to head to iterate through list again
		current = head;

		for (int i = 0; i < choice; i++) { //iterate through the linked list until at chosen record (by index)
			current = current->next;
		}

		system("cls");

		//display record selected for rating
		printf("RATING RECORD:\n\n");
		printf("Song:         %s\n", current->data.song);
		printf("Album:        %s\n", current->data.album);
		printf("Artist:       %s\n", current->data.artist);
		printf("Genre:        %s\n", current->data.genre);
		printf("Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
		printf("Rating:       %d\n", current->data.rating);
		printf("Times played: %d\n\n", current->data.timesPlayed);

		//get new rating and assign to newRating variable
		printf("Enter your new rating (1-5): ");
		scanf("%d", &newRating);

		//check if user entered a valid rating
		while (newRating < 1 || newRating > 5) {
			printf("\n\nInvalid input detected. Try again.\n");
			printf("Input: ");
			scanf("%d", &newRating);
		}

		//set current node rating to newRating
		current->data.rating = newRating;
	}
}

void play(Node* head) {
	Node* current = head;
	int index = 0; //important to use to iterate back to the record the user chooses if multiple records matching name are found
	int found = 0, choice = 0, attribute = 0;
	char song[64];

	//prompt song name
	printf("\nEnter the name of the song you would like to play.\n\n");
	printf("Input: ");

	scanf(" %[^\n]s", song); //get input including space

	printf("\nRESULTS");
	printf("\n--------------------------\n");

	while (current != NULL) { //iterate until the end of the list

		if (strcmp(song, current->data.song) == 0) { //print record with corresponding index if the song names match
			printf("%d)     Song:         %s\n", index, current->data.song);
			printf("       Album:        %s\n", current->data.album);
			printf("       Artist:       %s\n", current->data.artist);
			printf("       Genre:        %s\n", current->data.genre);
			printf("       Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
			printf("       Rating:       %d\n", current->data.rating);
			printf("       Times played: %d\n\n", current->data.timesPlayed);

			found++;
		}

		index++;
		current = current->next; //move to next node

	}

	if (found == 0) { //no song matching the search name is found
		printf("\nSorry, no songs matching this name were found.\n");
	}

	else {

		//prompt selection of songs matching the inputted name to play
		printf("\nEnter the number that corresponds with the song you would like to play\n\n");
		printf("Input: ");

		scanf("%d", &choice);

		//reset current to head to iterate through list again
		current = head;

		for (int i = 0; i < choice; i++) { //iterate through the linked list until at chosen record (by index)
			current = current->next;
		}

		//start playing from selected song
		while (current != NULL) {

			system("cls");

			printf("NOW PLAYING\n");
			printf("-----------------------------------------------------\n");
			printf("Song:   %s\n", current->data.song);
			printf("Album:  %s\n", current->data.album);
			printf("Artist: %s\n", current->data.artist);
			printf("Genre:  %s\n", current->data.genre);
			printf("Length: %d:%d\n", current->data.length.minutes, current->data.length.seconds);
			printf("Rating: %d\n", current->data.rating);

			if (current->next == NULL) printf("\n END OF PLAYLIST REACHED, GOING BACK TO MAIN MENU\n\n");

			Sleep(3000);

			current = current->next;
		}

	}
}

void insert(Node** head) {

	system("cls");

	//new record struct
	Record newData;
	
	//prompt user for data fields
	printf("NEW RECORD\n");
	printf("-----------------------------------\n");
	printf("Enter song:                    ");
	scanf(" %[^\n]s", newData.song);

	printf("Enter album:                   ");
	scanf(" %[^\n]s", newData.album);

	printf("Enter artist:                  ");
	scanf(" %[^\n]s", newData.artist);
	 
	printf("Enter genre:                   ");
	scanf(" %[^\n]s", newData.genre);
	 
	printf("Enter minutes:                 ");
	scanf("%d", &newData.length.minutes);

	printf("Enter seconds:                 ");
	scanf("%d", &newData.length.seconds);

	printf("Enter rating:                  ");
	scanf("%d", &newData.rating);

	printf("Enter number of times played:  ");
	scanf("%d", &newData.timesPlayed);

	//make new node using data
	Node* newHead = makeNode(newData);

	Node* oldHead = *head;

	//set new node next to old head
	newHead->next = *head;

	//set oldHead prev to the new head if old head is not null
	if (oldHead != NULL) oldHead->prev = newHead;

	//set new node to head
	*head = newHead;

}

void delete(Node** head) {
	Node* current = *head;
	int index = 0, found = 0;
	char song[64];

	//prompt song name
	printf("\nEnter the name of the song you would like to delete.\n\n");
	printf("Input: ");

	scanf(" %[^\n]s", song); //get input including space

	printf("\nRESULTS");
	printf("\n--------------------------\n");

	while (current != NULL) { //iterate until the end of the list

		if (strcmp(song, current->data.song) == 0) { //print record with corresponding index if the song names match
			printf("%d)     Song:         %s\n", index, current->data.song);
			printf("       Album:        %s\n", current->data.album);
			printf("       Artist:       %s\n", current->data.artist);
			printf("       Genre:        %s\n", current->data.genre);
			printf("       Length:       %d:%d\n", current->data.length.minutes, current->data.length.seconds);
			printf("       Rating:       %d\n", current->data.rating);
			printf("       Times played: %d\n\n", current->data.timesPlayed);

			found++;
		}

		index++;
		current = current->next; //move to next node

	}

	if (found == 0) { //no song matching the search name is found
		printf("\nSorry, no songs matching this name were found.\n");
	}

	else {

		int choice = 0;

		//prompt selection of songs matching the inputted name to play
		printf("\nEnter the number that corresponds with the song you would like to delete.\n\n");
		printf("Input: ");

		scanf("%d", &choice);

		//reset current to head to iterate through list again
		current = *head;

		for (int i = 0; i < choice; i++) { //iterate through the linked list until at chosen record (by index)
			current = current->next;
		} //now current is the record we need to delete

		if (current->prev == NULL && current->next == NULL) { //case - node being deleted is only node
			*head = NULL;
		}
		
		else if (current->prev == NULL && current->next != NULL) { //case - node being deleted is the head

			current->next->prev = NULL; //set next node (new head) prev to null
			*head = current->next; //set next node to be the new head

		}

		else if (current->next == NULL && current->prev != NULL) { //case - node being deleted is last node

			//only need to set last node (node before current) pnext to NULL
			current->prev->next = NULL;
		}

		else { //current is not the head or last node

			//"close the gaps" - set pnext of the previous node to the node after current and pprev of the next node to the node before prev
			current->prev->next = current->next;
			current->next->prev = current->prev;

		}

		//free the space of node being deleted
		free(current);
		current = NULL;
	}
}

//used in quicksort to seperate list into a "smaller" and "larger" list around pivot (last node)
Node* partition(Node* start, Node* end, int sortCrit) {
	Node* pivot = end; //our pivot will be the last node

	Node* current = start; //current is our iterating node

	Node* i = current->prev; //i keeps track of the first node "larger" than our pivot (where pivot's place will be)

	while (current != end) {
		
		if (strcmp(current->data.song, end->data.song) < 0 && sortCrit == 1) { //based on artist (A-Z)
			i = (i == NULL ? start : i->next); //i will stay at the last node of the "smaller" list, if find "small" node, then i will move to first "large" node in list and swap
			Record temp = i->data;
			i->data = current->data;
			current->data = temp;
		}
		 
		else if (strcmp(current->data.album, end->data.album) < 0 && sortCrit == 2) { //based on album title (A-Z)
			i = (i == NULL ? start : i->next); 
			Record temp = i->data;
			i->data = current->data;
			current->data = temp;
		}

		else if (current->data.rating < end->data.rating && sortCrit == 3) { //based on rating (1-5)
			i = (i == NULL ? start : i->next);
			Record temp = i->data;
			i->data = current->data;
			current->data = temp;
		}

		else if (current->data.timesPlayed > end->data.timesPlayed && sortCrit == 4) { //based on times played (most to least)
			i = (i == NULL ? start : i->next); 
			Record temp = i->data;
			i->data = current->data;
			current->data = temp;
		}

		current = current->next;

	}

	i = (i == NULL ? start : i->next); //swap first node in "large" list with pivot so that pivot is in the correct place
	Record temp = i->data;
	i->data = pivot->data;
	pivot->data = temp;
	return i;
}

//recursive function to quicksort list
void quickSort(Node* start, Node* end, int sortCrit) {
	if (end != NULL && start != end && start != end->next) { //keep recursively splitting list up and sorting until reach base case lists (lists of size 2)
		Node* p = partition(start, end, sortCrit);
		quickSort(start, p->prev, sortCrit); //sort first half
		quickSort(p->next, end, sortCrit); //sort second half
	}
}

void sort(Node* head) {
	int sortCrit = 0; //corresponds with user-selected sorting criteria
	Node* end = head; //end will be last node

	while (end->next != NULL) { //iterate until last node
		end = end->next;
	}
	
	printf("SORT METHODS\n");
	printf("-------------------------------------------------------------------------------\n");
	printf("(1) based on artist (A-Z)\n");
	printf("(2) based on album title (A-Z)\n");
	printf("(3) based on rating (1-5)\n");
	printf("(4) based on times played (most to least)\n\n");
	printf("Enter the number corresponding with the sort method you would like to use: ");

	scanf("%d", &sortCrit);

	//invalid input catch
	while (sortCrit < 1 || sortCrit > 4) {
		printf("\n\nInvalid input detected. Try again.\n");
		printf("Input: ");

		scanf("%d", &sortCrit);
	}

	quickSort(head, end, sortCrit); //recursively sort using quicksort
}

void shuffle(Node* head) {
	Node* current = head; //iterator node
	
	int count = 0; //size of list

	int index = 0;

	while (current != NULL) { //count number of elements in list
		count++;
		current = current->next;
	}

	int* array = malloc(sizeof(int) * count); //create array of same length as list
	for (int i = 0; i < count; i++) //assign default value to -1
		*(array + i) = -1;

	for (int i = 0; i < count; i++) { //assign each number between 0 and count to a unique index in array
		
		do {
			index = rand() % count;
 		} while (*(array + index) != -1); //keep randomly selecting number until find an empty spot in array

		*(array + index) = i; //assign number to empty spot
	} //now have random order to play songs in array

	//reset current and index and reuse for playing through the shuffled order
	current = head;
	index = 0;

	for (int i = 0; i < count; i++) { //iterate through array and play songs according to number stored in array

		while (index < *(array + i)) { //move forward through list while target song index is higher than current index is
			current = current->next;
			index++;
		}

		while (index > *(array + i)) { //move backwards through list while target song index is lower than current index is
			current = current->prev;
			index--;
		}

		system("cls");

		printf("NOW PLAYING\n");
		printf("-----------------------------------------------------\n");
		printf("Song:   %s\n", current->data.song);
		printf("Album:  %s\n", current->data.album);
		printf("Artist: %s\n", current->data.artist);
		printf("Genre:  %s\n", current->data.genre);
		printf("Length: %d:%d\n", current->data.length.minutes, current->data.length.seconds);
		printf("Rating: %d\n", current->data.rating);

		if (i == count - 1) printf("\nEND OF PLAYLIST REACHED, GOING BACK TO MAIN MENU\n\n");

		Sleep(3000);
	}

}