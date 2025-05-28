//mycrossword.c - Jack Roberts
//November 17, 2024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

char *strfry(char *string); //Prevents a warning from happening when strfry is used in the program

#define EMPTY '.' //Declaring '.' to be an empty space

int boardSize = 15;
int maxWords = 20; 


//Define all the function prototypes. All the function names that end in '3' correspond to when argc = 3. 
void boardFunc(char gameArr[boardSize][boardSize]);
void upperFunc(char inpArr[boardSize][boardSize]);
void wordSort(char inpArr[boardSize][boardSize], int count);
void firstTwo(char inpArr[boardSize][boardSize], char puzArr[boardSize][boardSize], char solArr[boardSize][boardSize], int boardSize);
void scramble(char inpArr[boardSize][boardSize], char scrambledArr[boardSize][boardSize], int count);
bool canPlaceWord(char board[][boardSize], int boardSize, char *word, int row, int col, bool vertical);
void placeWord(char board[][boardSize], int boardSize, char *word, int row, int col, bool vertical);
void placeRemainingWords(char board[][boardSize], int boardSize, char inpArr[][boardSize], int wordCount);
void updatePuzzleArray(char solArr[][boardSize], char puzArr[][boardSize], int boardSize);
void printWordLocations(char board[][boardSize], char inpArr[][boardSize], char scrambledArr[][boardSize], int wordCount, int boardSize);
void printWordLocations3(char board[][boardSize], char inpArr[][boardSize], char scrambledArr[][boardSize], int wordCount, int boardSize, FILE *copyFile);
void boardFunc3(char gameArr[][boardSize], FILE *copyFile);
void firstTwo3(char inpArr[][boardSize], char puzArr[][boardSize], char solArr[][boardSize], int boardSize, FILE *copyFile);



int main(int argc, char *argv[]) {

	int count = 0; //Will hold the number of words entered by the user

	char inpWord[boardSize];
	char inpArr[maxWords][boardSize]; //Contains all of the words inputted by the user
	char scrambledArr[maxWords][boardSize]; //Will contain all the scrambled words

	for (int i = 0; i < maxWords; i++) {
		for (int j = 0; j < boardSize; j++) {
			inpArr[i][j] = ' ';
		}
	}

	char solArr[boardSize][boardSize];
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			solArr[i][j] = '.';
		}
	}

	char puzArr[boardSize][boardSize];
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			puzArr[i][j] = '#';
		}
	}

	for (int i = 0; i < maxWords; i++) {
		for (int j = 0; j < boardSize; j++) {
			scrambledArr[i][j] = ' ';
		}
	}

	printf("\n");
	printf("Anagram Crossword Puzzle Generator\n");
	printf("-----------------------------------\n");
	printf("\n");

	if (argc == 1) { //Mode 1 of the game

		printf("Enter a list of words: \n");
		while (1) { //Takes in all of the words entered by the user and stores them in the inpArr
			int letter = 1;
			scanf("%s", inpWord);
			if (strcmp(inpWord, ".") == 0) break;
			
			for (int i = 0; i < strlen(inpWord); i++) { //Checks formatting of the word
				if (isalpha(inpWord[i]) == 0) {
					letter = 0;
					printf("Word must contain only letters\n");
					break;
				}
			}
			if ((strlen(inpWord) >= 2) && (strlen(inpWord) <= 15) && (count < maxWords) && letter == 1) {
				strcpy(inpArr[count], inpWord);
				count++;
			}
			else printf("Word does not meet input criteria\n");
		}
		
		//All of the functions called below define the user interface and logic behind the game. I have made comments for each of them, respectively. 
		upperFunc(inpArr);
		wordSort(inpArr, count);

		firstTwo(inpArr, puzArr, solArr, boardSize);
		placeRemainingWords(solArr, boardSize, inpArr, count);
		updatePuzzleArray(solArr, puzArr, boardSize);

		printf("Solution:\n");
		boardFunc(solArr);

		printf("\n");
		printf("\n");
		printf("Crossword Puzzle:\n");
		
		boardFunc(puzArr);

		printf("\n");
		printf("Clues:\n");
		printf("\n");

		scramble(inpArr,scrambledArr, count);
		printWordLocations(solArr, inpArr, scrambledArr, count, boardSize);

	}

	if (argc == 2) { //Mode 2 of the game
		FILE *fp;
		char filename[30];
		strcpy(filename, argv[1]);
		
		fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("Can't open the file\n");
			return 1;
		}

		while(!feof(fp)) { //Reads through the file and stores each word in the inpArr
			int letter = 1;
			fscanf(fp, "%s", inpWord);
			if (strcmp(inpWord, ".") == 0) break;
			
			for (int i = 0; i < strlen(inpWord); i++) { //Checks formatting of the word
				if (isalpha(inpWord[i]) == 0) {
					letter = 0;
					printf("Word must contain only letters\n");
					break;
				}
			}
			if ((strlen(inpWord) >= 2) && (strlen(inpWord) <= 15) && (count < maxWords) && letter == 1) {
				strcpy(inpArr[count], inpWord);
				count++;
			}
			else printf("%s does not meet input criteria\n", inpWord);
		}

		//Same sequence of functions called as in Mode 1
		upperFunc(inpArr);
		wordSort(inpArr, count);

		firstTwo(inpArr, puzArr, solArr, boardSize);
		placeRemainingWords(solArr, boardSize, inpArr, count);
		updatePuzzleArray(solArr, puzArr, boardSize);

		printf("Solution:\n");
		boardFunc(solArr);

		printf("\n");
		printf("\n");
		printf("Crossword Puzzle:\n");
		
		boardFunc(puzArr);

		printf("\n");
		printf("Clues:\n");
		printf("\n");

		scramble(inpArr,scrambledArr, count);
		printWordLocations(solArr, inpArr, scrambledArr, count, boardSize);

	}

	if (argc == 3) { //Mode 3 of the game
		FILE *fp;
		char filename[30];
		char newFile[30]; //Creates a new file in which the results of the game will be stored
		strcpy(filename, argv[1]);
		strcpy(newFile, argv[2]);
		
		fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("Can't open the file\n");
			return 1;
		}

		while(!feof(fp)) { //Reads all of the words from the given file
			int letter = 1;
			fscanf(fp, "%s", inpWord);
			if (strcmp(inpWord, ".") == 0) break;
			
			for (int i = 0; i < strlen(inpWord); i++) {
				if (isalpha(inpWord[i]) == 0) {
					letter = 0;
					printf("Word must contain only letters\n");
					break;
				}
			}
			if ((strlen(inpWord) >= 2) && (strlen(inpWord) <= 15) && (count < maxWords) && letter == 1) {
				strcpy(inpArr[count], inpWord);
				count++;
			}
			else printf("%s does not meet input criteria\n", inpWord);
		}
		fclose(fp);

		//Similar sequence of functions as in modes 1 and 2. However, a few of these functions have been customized so as to export the results directly to the new file. 
		FILE *copyFile;
		copyFile = fopen(newFile, "w");

		upperFunc(inpArr);
		wordSort(inpArr, count);

		firstTwo3(inpArr, puzArr, solArr, boardSize, copyFile);
		placeRemainingWords(solArr, boardSize, inpArr, count);
		updatePuzzleArray(solArr, puzArr, boardSize);
		
		fprintf(copyFile, "Solution:\n");
		boardFunc3(solArr, copyFile);

		fprintf(copyFile, "\nCrossword Puzzle:\n");
		boardFunc3(puzArr, copyFile);

		fprintf(copyFile, "\nClues:\n");
		scramble(inpArr,scrambledArr, count);
		printWordLocations3(solArr, inpArr, scrambledArr, count, boardSize, copyFile);
		fclose(copyFile);
	
	}

	return 0;
}





void boardFunc(char gameArr[boardSize][boardSize]) { //This function creates the game boards, each of which are outlined by an exterior box design. 

	for(int i = 0; i < boardSize + 2; i++) {
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

	for (int i = 0; i < boardSize + 2; i++) {
		printf("-");
	}
	printf("\n");
}

void upperFunc(char inpArr[boardSize][boardSize]) { //This function converts all letters stored in inpArr to uppercase

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			inpArr[i][j] = toupper(inpArr[i][j]);
		}
	}
}

void wordSort(char inpArr[boardSize][boardSize], int count) { //This function uses a temp variable to sort all the words in the inpAr from greatest character length to shortest character length.

	for (int i = 0; i < count-1; i++) {
		for (int j = 0; j < count - i - 1; j++) {
			 if (strlen(inpArr[j]) < strlen(inpArr[j+1])) {
				char tempWord[boardSize];
				strcpy(tempWord, inpArr[j]);
				strcpy(inpArr[j], inpArr[j+1]);
				strcpy(inpArr[j+1], tempWord);
			}
		}
	}
} 

void firstTwo(char inpArr[][boardSize], char puzArr[][boardSize], char solArr[][boardSize], int boardSize) { //This function places the first two words in the inpArr onto the game boards. The first word is placed horizontally, and the second word is placed vertically. 
    int startPt = (boardSize - strlen(inpArr[0])) / 2;
    int found = 0;

    // Places the first word horizontally in the middle of the board
    for (int i = 0; i < strlen(inpArr[0]); i++) {
        puzArr[boardSize / 2][startPt + i] = ' ';
        solArr[boardSize / 2][startPt + i] = inpArr[0][i];
    }

    // This checks for an intersection between the first two words
    for (int i = 0; i < strlen(inpArr[0]); i++) {
        for (int j = 0; j < strlen(inpArr[1]); j++) {
            if (inpArr[0][i] == inpArr[1][j]) {
                found = 1;

                // This places the second word vertically
                for (int k = 0; k < strlen(inpArr[1]); k++) {
                    int row = boardSize / 2 - j + k;
                    puzArr[row][startPt + i] = ' ';
                    solArr[row][startPt + i] = inpArr[1][k];
                }
                break;
            }
        }
        if (found) break;
    }

    // An error message is displayed if the two words do not intersect
    if (!found) {
        printf("The two words do not intersect\n");
    }
}


void scramble(char inpArr[boardSize][boardSize], char scrambledArr[boardSize][boardSize], int count) { //This function uses strfry to scramble each of the words

	int i;
	for (i = 0; i < count; i++) {	
		for (int j = 0; j < boardSize; j++) {
			scrambledArr[i][j] = inpArr[i][j]; //Assigns all values in inpArr equal to all values in scrambledArr, initially
		}
	}

	for (int i = 0; i < count; i++) {
		strfry(scrambledArr[i]);
	}
}

bool canPlaceWord(char board[][boardSize], int boardSize, char *word, int row, int col, bool vertical) { //This function returns true if the word can be placed and false otherwise
    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++) {
        int r = vertical ? row + i : row; //If the word is vertical, the row will change
        int c = vertical ? col : col + i; //If the word is vertical, the column will change

        if (r < 0 || r >= boardSize || c < 0 || c >= boardSize) return false; //Checks the boundaries of the board and returns false if the boundary conditions are not met

        if (board[r][c] != EMPTY && board[r][c] != word[i]) return false; //Checks for intersection

        if (board[r][c] == EMPTY) {//Checks to make sure the word can be placed with enough separation between other words
            if (vertical) {
                if ((c > 0 && board[r][c - 1] != EMPTY) || (c < boardSize - 1 && board[r][c + 1] != EMPTY)) return false;
            } else {
                if ((r > 0 && board[r - 1][c] != EMPTY) || (r < boardSize - 1 && board[r + 1][c] != EMPTY)) return false;
            }
        }
    }

    if (vertical) {//Checks to make sure there is enough separation around the beginning and end of the word
        if (row > 0 && board[row - 1][col] != EMPTY) return false;
        if (row + wordLength < boardSize && board[row + wordLength][col] != EMPTY) return false;
    } else {
        if (col > 0 && board[row][col - 1] != EMPTY) return false;
        if (col + wordLength < boardSize && board[row][col + wordLength] != EMPTY) return false;
    }

    return true;
}


void placeWord(char board[][boardSize], int boardSize, char *word, int row, int col, bool vertical) { //Places the word on the board
    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++) { //Again, if the word is being placed vertically, the column will stay constant. If it is being placed horizontally, the row will stay constant
        int r = vertical ? row + i : row;
        int c = vertical ? col : col + i;
        board[r][c] = word[i];
    }
}


void placeRemainingWords(char board[][boardSize], int boardSize, char inpArr[][boardSize], int wordCount) { //This function is used to place all words besides the first two.
    for (int w = 2; w < wordCount; w++) { // Start from the third word in inpArr
        char *word = inpArr[w];
        int wordLength = strlen(word);
        bool placed = false; //Defines placed as a boolean variable to break the for loop when needed.

        // Try to place the word by intersecting with an existing letter
        for (int row = 0; row < boardSize && !placed; row++) {
            for (int col = 0; col < boardSize && !placed; col++) {
                for (int i = 0; i < wordLength; i++) { // Checks each letter of the word
                  
                    if (board[row][col] == word[i] && canPlaceWord(board, boardSize, word, row, col - i, false)) { //The word is placed horizontally if these criteria are met
                        placeWord(board, boardSize, word, row, col - i, false);
                        placed = true;
                        break;
                    }
                    
                    if (board[row][col] == word[i] && canPlaceWord(board, boardSize, word, row - i, col, true)) { //The word is placed vertically if these criteria are met
                        placeWord(board, boardSize, word, row - i, col, true);
                        placed = true;
                        break;
                    }
                }
            }
        }

        
        if (!placed) { //Notifies the user if the word cannot be placed on the board
            printf("Could not place word '%s' on the board\n", word);
            break;
        }
    }
}

void updatePuzzleArray(char solArr[][boardSize], char puzArr[][boardSize], int boardSize) { //This function updates the puzArr with the new words based on the solArr
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (solArr[i][j] != EMPTY) {
                puzArr[i][j] = ' '; //Any elements in solArr that contain letters are replaced with spaces in puzArr
            } else {
                puzArr[i][j] = '#'; //Empty cells in solArr are defined as '#' symbols in puzArr
            }
        }
    }
}

void printWordLocations(char board[][boardSize], char inpArr[][boardSize], char scrambledArr[][boardSize], int wordCount, int boardSize) { //This function prints the locations of each of the words and displays the clues as well via the scrambledArr
	
	int i = 0;
   for (int w = 0; w < wordCount; w++) {
       char *word = inpArr[w]; //Points to the current word being analyzed
       int wordLength = strlen(word);
       bool found = false; //Keeps track of whether or not the word has been found on the board

       
       for (int row = 0; row < boardSize && !found; row++) { //Goes through each row of the board until the word is found
           for (int col = 0; col < boardSize && !found; col++) { //Goes through each of the columns until the word is found
               // Checks for horizontal placement
               if (col + wordLength <= boardSize) { //Checks to see whether the word fits horizontally
                   bool matches = true;
                   for (int i = 0; i < wordLength; i++) { //Makes sure each letter in the word corresponds to what is on the board
                       if (board[row][col + i] != word[i]) {
                           matches = false;
                           break;
                       }
                   }
                   if (matches) { //If the word matches the board, the location is displayed
                       printf("%d, %d Across %s\n", col + 1, row + 1, scrambledArr[i]);
                       found = true;
                       break;
                   }
               }

               // Checks for vertical placement
               if (row + wordLength <= boardSize) { // Ensures that the  word fits vertically on the board
                   bool matches = true;
                   for (int i = 0; i < wordLength; i++) {
                       if (board[row + i][col] != word[i]) { //Checks to make sure each letter in the word corresponds to what is on the board
                           matches = false;
                           break;
                       }
                   }
                   if (matches) { //The location on the board is shown if the word matches what is on the board
                       printf("%d, %d Down %s\n", col + 1, row + 1, scrambledArr[i]);
                       found = true;
                       break;
                   }
               }
           }
       }

       if (!found) { //Displays an error message if the word is not found on the board
           printf("Word '%s' not found on the board.\n", word);
       }
	i++;
   }
}

void boardFunc3(char gameArr[][boardSize], FILE* copyFile) { //This is the same as boardFunc, but it is designed to print all outputs directly to the new file when argc = 3.
    for (int i = 0; i < boardSize + 2; i++) {
        fprintf(copyFile, "-");
    }
    fprintf(copyFile, "\n");

    for (int i = 0; i < boardSize; i++) {
        fprintf(copyFile, "|");
        for (int j = 0; j < boardSize; j++) {
            fprintf(copyFile, "%c", gameArr[i][j]);
        }
        fprintf(copyFile, "|\n");
    }

    for (int i = 0; i < boardSize + 2; i++) {
        fprintf(copyFile, "-");
    }
    fprintf(copyFile, "\n");
}

void printWordLocations3(char board[][boardSize], char inpArr[][boardSize], char scrambledArr[][boardSize], int wordCount, int boardSize, FILE *copyFile) { //This is the same as printWordLocations, but it is designed to print everything directly to the new file when argc = 3. 
    int i = 0;
    for (int w = 0; w < wordCount; w++) {
        char *word = inpArr[w];
        int wordLength = strlen(word);
        bool found = false;

        for (int row = 0; row < boardSize && !found; row++) {
            for (int col = 0; col < boardSize && !found; col++) {
                if (col + wordLength <= boardSize) {
                    bool matches = true;
                    for (int i = 0; i < wordLength; i++) {
                        if (board[row][col + i] != word[i]) {
                            matches = false;
                            break;
                        }
                    }
                    if (matches) {
                        fprintf(copyFile, "%d, %d Across %s\n", col + 1, row + 1, scrambledArr[w]); 
                        found = true;
                        break;
                    }
                }

                if (row + wordLength <= boardSize) {
                    bool matches = true;
                    for (int i = 0; i < wordLength; i++) {
                        if (board[row + i][col] != word[i]) {
                            matches = false;
                            break;
                        }
                    }
                    if (matches) {
                        fprintf(copyFile, "%d, %d Down %s\n", col + 1, row + 1, scrambledArr[w]); 
                        found = true;
                        break;
                    }
                }
            }
        }

        if (!found) {
            fprintf(copyFile, "Word '%s' not found on the board.\n", word);
        }
        i++;
    }
}

void firstTwo3(char inpArr[][boardSize], char puzArr[][boardSize], char solArr[][boardSize], int boardSize, FILE *copyFile) { //This is the same as the firstTwo words function, but it is designed to print all output directly to the new file when argc = 3. 
    int startPt = (boardSize - strlen(inpArr[0])) / 2;
    int found = 0;

    for (int i = 0; i < strlen(inpArr[0]); i++) {
        puzArr[boardSize / 2][startPt + i] = ' ';
        solArr[boardSize / 2][startPt + i] = inpArr[0][i];
    }

    for (int i = 0; i < strlen(inpArr[0]); i++) {
        for (int j = 0; j < strlen(inpArr[1]); j++) {
            if (inpArr[0][i] == inpArr[1][j]) {
                found = 1;

                for (int k = 0; k < strlen(inpArr[1]); k++) {
                    int row = boardSize / 2 - j + k;
                    puzArr[row][startPt + i] = ' ';
                    solArr[row][startPt + i] = inpArr[1][k];
                }
                break;
            }
        }
        if (found) break;
    }

    if (!found) {
        fprintf(copyFile, "The two words '%s' and '%s' do not intersect.\n", inpArr[0], inpArr[1]);
    }
}

