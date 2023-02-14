/*
* Name: Brandon Xu
* Project: DigitalMusicManager
* File: main.c
* Date: 2/1/23
* Description: Main file for execution of the program. Contains the main loops and overarching logic.
*/

#include "testheader.h"
#include "header.h"

int main(void) {

	/*
	
	TEST FUNCTIONS

	insertTest();
	deleteTest();
	shuffleTest();

	*/

	//variable declarations
	int choice = 0;
	Node* head = NULL;

	//needed for random number generation for shuffle function
	srand(time(NULL));

	//open file for read
	FILE* file = fopen("musicPlayList.csv", "r");

	do {

		//display and get menu choice
		choice = handleMainMenu();

		switch (choice) {

			case 1: 
				//read file into list
				head = load(file);

				//close file
				fclose(file);

				printf("Operation executed successfully!\n\n");

				break; 

			case 2: 
				//open file for overwrite
				file = fopen("musicPlayList.csv", "w");

				store(file, head);

				//close file
				fclose(file);

				printf("Operation executed successfully!\n\n");

				break;

			case 3:
				//display function fully handles this case 
				display(head);
				break;

			case 4:
				insert(&head);
				break;

			case 5:
				delete(&head);
				break;

			case 6:
				edit(head);
				break;

			case 7:
				sort(head);
				break;

			case 8: 
				rate(head);
				break;

			case 9:
				play(head);
				break;

			case 10:
				shuffle(head);
				break;
		}

		//pause
		system("pause");

		//clear screen
		system("cls");

	} while (choice != 11);

	//open file for overwrite
	file = fopen("musicPlayList.csv", "w");

	//saves most recent list to file
	store(file, head);
	
	//exit message
	printf("Goodbye!");

	//close file
	fclose(file);


	return 0;
}

/*
"Swift, Taylor",1989,Shake it Off,Pop,3:35,12,3
Drake,NOTHING WAS THE SAME,Own it,Rap,3:23,3,3
Drake,YOU WELCOME,The Motto,Rap,4:13,7,4
"Perri, Christina",HEAD OF HEART,Trust,Pop,2:35,3,5
"Bieber, Justin",PURPOSE,No Sense,Pop,4:12,6,1
Eminem,SHADYXV,Vegas,Rap,3:37,8,3
Adele,25,Remedy,Pop,4:11,24,4
"Swift, Taylor",RED,Stay Stay Stay,Pop,4:42,5,1
"Brooks, Garth",FRESH HORSES,The Old Stuff,Country,2:57,11,2
*/