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
		current->prev = current;
	}
}

void printList(Node* head) {
	Node* current = head;
	int count = 1;

	//iterate through list until end of list is reached
	while (current != NULL) {
		printf("%d. %s, %s, %s, %s, %d min %d sec, %d, %d\n", count, current->data.artist, 
			current->data.album, current->data.song, current->data.genre, current->data.length.minutes, 
			current->data.length.seconds, current->data.timesPlayed, current->data.rating);
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
	strcpy(data.genre, token);

	//gets song name
	token = strtok(NULL, ",");
	strcpy(data.song, token);

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

	//make head node of list
	data = parseLine(file);
	Node* head = makeNode(data);

	data = parseLine(file);

	while (data.rating != 0) { //parse until the end of the file

		insertEnd(&head, data); //insert nodes with the records at the end of the list

		data = parseLine(file);
	}

	return head;
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

		scanf("%s", &search); //get input

		while (current != NULL) { //iterate until the end of the list

			if (strcmp(search, current->data.artist) == 0) { //print record if the artist names match
				printf("%d. %s, %s, %s, %s, %d min %d sec, %d, %d\n", count, current->data.artist,
					current->data.album, current->data.song, current->data.genre, current->data.length.minutes,
					current->data.length.seconds, current->data.timesPlayed, current->data.rating);
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

	scanf("%s", &artist); //get input

	printf("\nRESULTS");
	printf("\n--------------------------\n");

	while (current != NULL) { //iterate until the end of the list

		if (strcmp(artist, current->data.artist) == 0) { //print record with corresponding index if the artist names match
			printf("(%d)   %s, %s, %s, %s, %d min %d sec, %d, %d\n", index, current->data.artist,
				current->data.album, current->data.song, current->data.genre, current->data.length.minutes,
				current->data.length.seconds, current->data.timesPlayed, current->data.rating);

			found++;
		}

		index++;
		current = current->next; //move to next node
	
	}

	if (found == 0) { //no artist matching the search name is found
		printf("\nSorry, no records matching this artist were found.\n");
	}

	else {
		printf("\nEnter the number that corresponds with the record you would like to edit\n\n");
		printf("Input: ");

		scanf("%d", &choice);

		//reset current to head to iterate through list again
		current = head;

		for (int i = 0; i < choice; i++) { //iterate through the linked list until at chosen record (by index)
			current = current->next;
		}

		system("cls");

		printf("Modifying record: %s, %s, %s, %s, %d min %d sec, %d, %d\n", current->data.artist,
			current->data.album, current->data.song, current->data.genre, current->data.length.minutes,
			current->data.length.seconds, current->data.timesPlayed, current->data.rating);

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

		switch (attribute) {
			case 1:
				printf("Enter the new artist name\n\n");
				printf("Input: ");
				scanf("%s", current->data.artist);
				break;

			case 2:
				printf("Enter the new album name\n\n");
				printf("Input: ");
				scanf("%s", current->data.album);
				break;

			case 3:
				printf("Enter the new song name\n\n");
				printf("Input: ");
				scanf("%s", current->data.song);
				break;

			case 4:
				printf("Enter the new genre name\n\n");
				printf("Input: ");
				scanf("%s", current->data.genre);
				break;

			case 5:
				printf("Enter minutes: \n\n");
				printf("Input: ");
				scanf("%d", current->data.length.minutes);

				printf("\n\nEnter seconds: \n\n");
				printf("Input: ");
				scanf("%d", current->data.length.seconds);
				break;

			case 6:
				printf("Enter new number of times played: \n\n");
				printf("Input: ");
				scanf("%d", current->data.timesPlayed);
				break;

			case 7:
				printf("Enter new rating: \n\n");
				printf("Input: ");
				scanf("%d", current->data.rating);
				break;
		}

		printf("\n\nModified record: %s, %s, %s, %s, %d min %d sec, %d, %d\n", current->data.artist,
			current->data.album, current->data.song, current->data.genre, current->data.length.minutes,
			current->data.length.seconds, current->data.timesPlayed, current->data.rating);
	}

}