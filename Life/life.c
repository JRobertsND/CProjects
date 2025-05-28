//Game of Life - Jack Roberts
//October 31, 2024

#include <stdio.h>
#include<string.h>
#include <unistd.h>
#include <ctype.h>

int boardSize = 40;

void boardFunc(char gameArr[boardSize][boardSize]);
void addCell(int, int, char [boardSize][boardSize], int);
void rmCell(int, int, char [boardSize][boardSize], int);
int countNeighbors(int, int, char[boardSize][boardSize], int);
void oneIteration(char[boardSize][boardSize], int);

int main(int argc, char *argv[]) {

	char input[50];
	int xcoord = 0;
	int ycoord = 0;
	char ltrEntry[10];
	int numInputs;
	
	int playing = 1;

	char gameArr[boardSize][boardSize];
	for (int i = 0; i < boardSize; i++) {
   	for (int j = 0; j < boardSize; j++) {
      	gameArr[i][j] = ' ';
    	}
	}

		if (argc == 1) {
			while (playing) {
				boardFunc(gameArr);
				printf("COMMAND: ");
				fgets(input, sizeof(input), stdin);
				numInputs = sscanf(input, "%s %d %d", ltrEntry, &xcoord, &ycoord);

				if (strcmp(ltrEntry,"a") == 0) {
					addCell(xcoord, ycoord, gameArr, boardSize);
				}
				else if (strcmp(ltrEntry,"r") == 0) {
					rmCell(xcoord, ycoord, gameArr, boardSize);
				}
				else if (strcmp(ltrEntry,"q") == 0) {
					playing = 0;
				}
				else if (strcmp(ltrEntry,"n") == 0) {
					oneIteration(gameArr, boardSize);	
				}
				else if (strcmp(ltrEntry,"p") == 0) {
					while (1) {
						oneIteration(gameArr, boardSize);
						boardFunc(gameArr);
						usleep(100000);
					}
				}
				else {
					printf("Please enter a valid input\n");
				}
			}
		}

		if (argc == 2) {
			FILE *fp;
			char filename[30];
			strcpy(filename, argv[1]);

			fp = fopen(filename, "r");
			if (fp == NULL) {
				printf("Can't open the file\n");
				return 1;
			}
		
			while(fgets(input, sizeof(input), fp) != NULL) {
				boardFunc(gameArr);
				usleep(100000);
				numInputs = sscanf(input, "%s %d %d", ltrEntry, &xcoord, &ycoord);

				if (strcmp(ltrEntry, "a") == 0) {
					addCell(xcoord, ycoord, gameArr, boardSize);
				}
				else if (strcmp(ltrEntry, "p") == 0) {
					while (1) {
						oneIteration(gameArr, boardSize);
						boardFunc(gameArr);
						usleep(100000);
					}
				}
				else {
					printf("Improper file format\n");
				}
			}		
		}

		if (argc > 2) {
			printf("Error: please enter the proper number of elements\n");
		}

	printf("Program terminated\n");

	
	return 0;
}

void boardFunc(char gameArr[boardSize][boardSize]) {
	for (int i = 0; i < boardSize + 2; i++) {
		printf("-");
	}
	printf("\n");

	for (int i = 0; i < boardSize; i++) {
		printf("|");
		for (int j = 0; j < boardSize; j++) {
			printf("%c", gameArr[i][j]);
		}
		printf("|\n");
	}

	for (int i = 0; i < boardSize; i++) {
		printf ("-");
	}
	printf("\n");

}

void addCell(int x, int y, char gameArr[boardSize][boardSize], int boardSize) {

	if (x >= 0 && x < boardSize && y >= 0 && y < boardSize){
		gameArr[x][y] =  'X';
	}
}

void rmCell (int x, int y, char gameArr[boardSize][boardSize], int boardSize) {

	if (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
		gameArr[x][y] = ' ';
	}
}

int countNeighbors (int x, int y, char gameArr[boardSize][boardSize], int boardSize) {

	int count = 0;

	if ((x-1>=0) && (gameArr[x-1][y] == 'X')) {
		count++;
	}
	if ((x+1<boardSize) && (gameArr[x+1][y] == 'X')) {
		count++;
	}
	if ((x-1>=0) && (y-1>=0) && (gameArr[x-1][y-1] == 'X')) {
		count++;
	}
	if ((x+1<boardSize) && (y+1<boardSize) && (gameArr[x+1][y+1] == 'X')) {
		count++;
	}
	if ((y-1>=0) && (gameArr[x][y-1] == 'X')) {
		count++;
	}
	if ((y+1<boardSize) && (gameArr[x][y+1] == 'X')) {
		count++;
	}
	if ((x-1>=0) && (y+1<boardSize) && gameArr[x-1][y+1] == 'X') {
		count++;
	}
	if ((x+1<boardSize) && (y-1>=0) && gameArr[x+1][y-1] == 'X') {
		count++;
	}

	return count;

}

void oneIteration (char gameArr[boardSize][boardSize], int boardSize) {

	char newArr[boardSize][boardSize];

	int count;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			count = countNeighbors(i, j, gameArr,boardSize);
			if (gameArr[i][j] == 'X')   {
				if (count == 2 || count == 3) {
					newArr[i][j] = 'X';
				}
				else {
					newArr[i][j] = ' ';
				}
					
			}
			else if (count == 3) {
				newArr[i][j] = 'X';
			}
			else {
				newArr[i][j] = ' ';
			}
		}
	}
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			gameArr[i][j] = newArr[i][j];
		}
	}	
}
