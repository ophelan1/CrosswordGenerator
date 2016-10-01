
/*################################ README ###############################################################
Jose submitted this program under his name, on time, last night. However, I just realized in lab today that the program we uploaded yesterday
doesn't work 100% of the time because we mixed up the 'xi' and 'xj' on line 391 when updating our variable names last minute. So I am reuploading the 
program under my name  ----- I know that this can't get us any points!!!! ----- But I decided to reupload the working version because: 
a.) We'd like you to know that the rest of the code in our official hand-in is all correct, and the problem actually is just a minor typo. But more importantly;
b.) Jose and I are SUPER proud of how the program turned out and were kind of looking forward to knowing we killed it and being all cocky about it with each other,
but turns out that's much less satisfyign when nobody else knows that it worked. So anyway,
Hope that's okay! Enjoy.*/


/* Laboratory 10: Crossword Anagram */
/* Lab by: Owen Phelan and Jose Badilla */

#include <stdio.h>
#include <math.h>
#include <unistd.h>	
#include <string.h>
#include <ctype.h>
#include <stdlib.h>		// for generating random numbers
#include <time.h>		// used for initiating random number generator
#define SIZE 20
#define LENGTH 16
#define DOWN -1
#define ACROSS 1

//#################################################### GLOBAL VARIABLES & DECLARATIONS ####################################################

char words[SIZE][LENGTH]; //WORDS   These three are independant Variables so we can manipulte them within functions easily
char board[15][15];
char boardDuplicate[15][15];
char boardDuplicate2[15][15];

int wordNum;

int onBoard[20][4]; 
/*This is an array that is going to keep track of which words are on the board, in what direction, and at what location. The first column is whether or 
not they're on the board - 0 = not on, 1 = on. The second column is the direction they are in - DOWN, ACROSS, or 0 if not on. 3rd = row, 4th = column*/

char clueStr[SIZE][LENGTH];

void sortWords(int);
void capitalize(void);
void printWords(void);
void makeBoard(void);
void printBoard(void);
void putOnBoard(int,int,int,int);
void lookForMatch(int,int,int,int);
int checkBoard(int);
void copyBoard1(void);
void copyBoard2(void);
void restoreBoard(void);
void checkBounds(int, int, int, int, int, int, int);
void printEmptyBoard(void);
void scramble_str(void);
void printClues(void);


//#######################################################################################################################################


int main(void){
	srand( time(NULL) );	//function initializes random number generator	
	
	int max = 0;
	int k = 0;
	int i,j;

	printf("Enter a list of words:\n");

	while (k < SIZE) {							//WHILE loop scans words to be added to the list
		scanf("%s", words[k]);	
		if(strlen(words[k]) > 15){
			printf("\nI'm Sorry, that word is too Long, Please Enter a different word\n");
			k--;
		}				

		if (strcmp(words[k], ".") == 0) {
			wordNum = k;
			break;
		}
		wordNum = k;
		k++;
	}
			

	printf("\n");
	sortWords(SIZE);
	capitalize();

	makeBoard();

	for( i = 0 ; i < wordNum ; i++){
		for(j = 0 ; j < 2 ; j++){
			onBoard[i][j] = 0;
		}
	}	

	/*SECTION 1: SO FAR: The program acepts the words, sorts them, capitalizes them, and prints them
	then it makes a board of all pound signs, and then prints that board. It also updates the array called "onBoard" which has
	the same number of elements as there are words, if the element = 0, the word is not on the board. 1, it is on the board*/

	int firstWordLength = strlen(words[0]);
	int firstWordStartCol = ( (15-firstWordLength) / 2);
	putOnBoard(0, ACROSS , firstWordStartCol , 7);

	/* SECTION 2: This part prints the first word onto the center of the board. It also sets onBoard[0][0] to 1, so we know its 
	on the board, and onBoard[0][1] to ACROSS, so we know the direction*/

	/* Variables for Next Section 
	prevWord - the last word put on the board, the word the match is being compared to.
	wordCount - the # word that is being compared. ( wordCount = 2 ) = "The third word is being checked"
	wordNum - the total number of words
	*/

	lookForMatch(0,wordNum, 7 , firstWordStartCol);

	/*SECTON 3: This part works so far. It starts with the second word (first word is already on the board so it will be ignored), and checks
	every letter of the second word against the first letter of the first word, and then against the second letter of the first word, and so on.
	When it finds a match, it prints the match onto the board. When doing this, the "onBoard" values of the matched word are updated
	so the program will not check that word again. */
	printf("\nTHE FINAL BOARD:\n");
	printBoard();
	printEmptyBoard();
	scramble_str();
	printClues();
}



//############   FUNCTIONS   ################//
void sortWords( int wordNum){

	int ix;
	char maxword[LENGTH];

	for (ix = 0; ix < SIZE; ix++) {		                 
		if ( strlen(words[ix+1]) > strlen(words[ix]) ) {			//Goes through the list and bubble sorts so it is from longest so shortest
			strcpy(maxword,words[ix+1]);
			strcpy(words[ix+1],words[ix]);
			strcpy(words[ix],maxword);
			ix = -1;												//Sets to (-1) so it starts again at zero once it increments
		}
	}
}

void capitalize(void){

	int ix;
	int ic;

	for(ix = 0; ix <= wordNum; ix++){
		for(ic = 0; ic < LENGTH; ic++){
			if (isalpha(words[ix][ic]) != 0) {
				words[ix][ic] = toupper(words[ix][ic]);
			}
		}
	}
}

void printWords(void){
	int i;
	for(i = 0 ; i <= wordNum ; i++){
		if (strcmp(words[i], ".") == 0){
			break;
		}
		else
		printf("%s\n",words[i]);
	}
}

void makeBoard(void){
	int i,j;
	for(i = 0 ; i < 15 ; i++){
		for(j = 0 ; j < 15 ; j++){
			board[i][j] = '-';
		}
	}
}

void printBoard(void){
	int i,j;
	printf("\n");
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

void lookForMatch(int prevWordNum, int totalWords, int prevRow , int prevCol){


	int i = 0, j = 0;
	int wordCount;
	int boardCheck;
	for(wordCount = 0 ; wordCount < totalWords ; wordCount++){      		

		if(onBoard[wordCount][0] == 0){											//If the word is not on the board, check to see if it is a match for the last word
				
			for( j = 0 ; j < strlen(words[wordCount]) && !onBoard[wordCount][0] ; j++){					//j = the letter of the possible match			
				for(i = 0 ; i < strlen(words[prevWordNum]) ; i++){				//i = the letter of the word we are finding a match for		
						
						if(words[prevWordNum][i] == words[wordCount][j]){	
							int newDirection = onBoard[prevWordNum][1] * -1;					
							checkBounds(totalWords, wordCount, newDirection, prevCol, prevRow, i, j);
						}
						if(onBoard[wordCount][0] == 1) break;
				}
			}

		}
	}
}

void putOnBoard(int wordNumber, int direction, int col, int row){

	copyBoard1();
	
	int i;
	int wordLength;
	wordLength = strlen(words[wordNumber]);
	
	switch (direction){

		case DOWN:
			for( i = 0 ; i < wordLength ; i++){
				board[row+i][col] = words[wordNumber][i];
			}
			break;
		case ACROSS:
			for( i = 0 ; i < wordLength ; i++){
				board[row][col+i] = words[wordNumber][i];
			}
		break;
	}


	onBoard[wordNumber][0] = 1;
	onBoard[wordNumber][1] = direction; // Questionable Part
	onBoard[wordNumber][2] = row;
	onBoard[wordNumber][3]= col;
}

int checkBoard(int wordMatch){
	int i, j;
	int check3 = 0;

	////// PART 1 ////////////////////////// Sees if the function overlaps with two words, and if so, if that is a problem (A little fancier then assigned ;) )


		for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			if(  (boardDuplicate[i][j]  != '-')  &&  (board[i][j] != boardDuplicate[i][j])  ){ //This means it overlapped a previous word another word. H
				restoreBoard();
				check3 = 1;
			}
		}
		}

	///////// PART 2 ////////////////////////// Sees if there are any words next to each other that shouldn't be. 

		//First Remove All the Vertical Words From the Board. Then, if there are more than two letters in a row vertically, that is an invalid placement.
		copyBoard2();
		for( i = 0 ; i < 20 ; i++){
			if(  (onBoard[i][0]==1)  &&  (onBoard[i][1]==DOWN)  ){
				int wordLength = strlen(words[i]);
				int row = onBoard[i][2];
				int col = onBoard[i][3];
				for(j = 0 ; j < wordLength ; j++){
					boardDuplicate2[row+j][col] = '-';			
				}

			}
		}

		for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			if(   (boardDuplicate2[i][j] != '-') && (boardDuplicate2[i+1][j] != '-')   ){
				restoreBoard();
				check3 = 1;
			}
		}
		}

		//Then Do the Same Thing, but in the horizontal direction

		copyBoard2();
		for( i = 0 ; i < 20 ; i++){
			if(  (onBoard[i][0]==1)  &&  (onBoard[i][1]==ACROSS)  ){
				int wordLength = strlen(words[i]);
				int row = onBoard[i][2];
				int col = onBoard[i][3];
				for(j = 0 ; j < wordLength ; j++){
					boardDuplicate2[row][col+j] = '-';			
				}

			}
		}
		for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			if(   (boardDuplicate2[i][j] != '-') && (boardDuplicate2[i][j+1] != '-')   ){
				restoreBoard();
				check3 = 1;
			}
		}
		}

		
		////////Finally, see if any of the heads of words interfere with the middle or heads of other words //////

		int rowStart = onBoard[wordMatch][2];
		int colStart = onBoard[wordMatch][3];
		int letterNum = strlen(words[wordMatch]);
		int rowFinish = rowStart + letterNum;
		int colFinish = colStart + letterNum;

		if(onBoard[wordMatch][1] == DOWN){											//If it is in the down direction
			if(  (rowStart != 0) && (board[rowStart-1][colStart] != '-')  ){			//If the first letter is not in the first row and the spot above it is taken
				restoreBoard();
				check3 = 1;
			} 
			else if(  (rowFinish+1 != 15)  && (board[rowFinish][colStart] != '-')  ){
				restoreBoard();
				check3 = 1;
			}
		}
		else if(onBoard[wordMatch][1] == ACROSS){											//If it is in the Accross direction
			if(  (colStart != 0) && (board[rowStart][colStart-1] != '-')  ){			//If the first letter is not in the first row and the spot above it is taken
				restoreBoard();
				check3 = 1;
			} 
			else if(  (colFinish+1 != 15)  && (board[rowStart][colFinish] != '-')  ){
				restoreBoard();
				check3 = 1;
			}
		}
return(check3);
}

void copyBoard1(void){	
	int i, j;
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			boardDuplicate[i][j] = board[i][j];
		}
	}
}

void copyBoard2(void){	
	int i, j;
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			boardDuplicate2[i][j] = board[i][j];
		}
	}
}

void restoreBoard(void){
	int k, l;
	for(k = 0 ; k < 15 ; k++){
		for(l = 0 ; l < 15 ; l++){
				board[k][l] = boardDuplicate[k][l];
		}
	}
}

void checkBounds(int totalWords, int matchNum, int dir, int prevCol, int prevRow, int xi, int xj){
	int check = 0;
	int check2 = 1;
	int wordLength = strlen(words[matchNum]);

	//Checks to See if the word is going to start out of bounds. If it is, it does not plot it.
	if(dir == ACROSS){
		if((prevCol - xi) < 0 ){
			check = 1;
		}
		else if(  ((prevRow - xi) + wordLength) > 15  ){
			check = 1;
		}
	}
	else if(dir == DOWN){
		if((prevRow - xj) < 0 ){
			check = 1;
		}
		else if(  ((prevRow - xj) + wordLength) > 15  ){
			check = 1;
		}
	}

	//If the word is not going to start out of bounds, it is plotted and then checked to see if it causes problems
	if(check == 0){
		
		check2 = 0; 
		if(dir == ACROSS){
			putOnBoard(matchNum, dir, prevCol - xj , prevRow + xi);
		}
		else if(dir == DOWN){
			putOnBoard(matchNum, dir, prevCol + xi , prevRow - xj);
		}


		check2 = checkBoard(matchNum); // if there is an issue, check2 == 1, and the word will be removed and onBoard reset.
	

		if (check2 == 0) { 
			lookForMatch( matchNum, totalWords, onBoard[matchNum][2], onBoard[matchNum][3] ); //Function calls itself recursively	
		}
		else if (check2 == 1){
			onBoard[matchNum][0] = 0;				// These two are being set to zero because the word was taken off the board
			onBoard[matchNum][1] = 0;
			onBoard[matchNum][2] = 0;
			onBoard[matchNum][3] = 0;
		}
	}
}

/* The following functions provide the empty board to serve as reference to the user and the other two functions:

	scramble_str() --> scrambles the strings (creates an anagram of word)
	printClues()   --> formally prints the clues with the row and column number, orientation and corresponding anagram */

void scramble_str(void){

	int iw = 0;
	int ic = 0;
	int strLen;

	int in;

	int det[LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int *ptr[LENGTH];	//randomizes 15 pointers
	char *ptrchar;
	int test;

	while(iw < wordNum){
			strLen = strlen(words[iw]);
			ptrchar = &words[iw][0];
			ic = 0;
		while(*ptrchar != '\0') {
			test = rand() % strLen;
			det[ic] = test;
			ptr[ic] = &det[ic];	//assign the address of an integer (use * to point to value)

			if (ic > 0) {
				for (in = ic - 1; in >= 0; in--){
					if (*ptr[ic] == *ptr[in]) {
						test = rand() % strLen;
						det[ic] = test;
						ptr[ic] = &det[ic];
						in = ic;
					}
				}
			}

			clueStr[iw][ic] = words[iw][*ptr[ic]];

			ptrchar++;
			ic++;
		}

		*ptrchar = '\0';
		iw++;
	}
}

void printEmptyBoard(void){
	int i,j;
	printf("\n");
	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){
			if (isalpha(board[i][j]) != 0) {
				printf("  ");	
			}
			else {
				printf("# ");	
			}
			
		}
		printf("\n");
	}
}

void printClues(void){
	int i,j,k = 0, n = 0, w, down = 0;
	int strLen;

	char cmpStr[LENGTH];
	char *ptrcmp; //compares to find out which string it is
	char clueWords[SIZE][LENGTH];

	printf("\n");
	printf("Clues:\n");
	printf("\n");

	for( i = 0 ; i < 15 ; i++){
		for( j = 0 ; j < 15 ; j++){	
																				
			if ( ((isalpha(board[i][j-1]) == 0) || (i == 0)) && (isalpha(board[i][j]) != 0) && (isalpha(board[i][j+1]) != 0) ) {

				ptrcmp = &board[i][j];								// 'if statement' determines the presence of the beginning of a string
				k = 0;												// 		by identfying that there are no alphabetical characters preceeding it,
				while(*ptrcmp != '-') {								//				that it is an alphabetical character,and that there ARE
					cmpStr[k] = *ptrcmp;							//						alphabetical characters proceeding it
					k++;
					ptrcmp++;
				}
				cmpStr[k] = '\0';
				
				for(w = 0 ; w < wordNum ; w++) {
					if (strcmp(cmpStr, words[w]) == 0) {
						strcpy(clueWords[n], clueStr[w]);
						n++;
						break;
					}
				}
				printf("%-2d, %-3d %-4s %-5s\n", i+1, j+1, "Across", clueWords[n-1]);
			}

			else if ( ((isalpha(board[i-1][j]) == 0) || (j == 0)) && (isalpha(board[i][j]) != 0) && (isalpha(board[i+1][j]) != 0) ) {

				ptrcmp = &board[i][j];
				k = 0, down = 0;
				while(*ptrcmp != '-') {
					cmpStr[k] = *ptrcmp;
					down++;
					ptrcmp = &board[i+down][j];
					k++;
				}
				cmpStr[k] = '\0';
				
				for(w = 0 ; w < wordNum ; w++) {
					if (strcmp(cmpStr, words[w]) == 0) {
						strcpy(clueWords[n], clueStr[w]);
						n++;
						break;
					}
				}
				printf("%-2d, %-3d %-6s %-5s\n", i+1, j+1, "Down", clueWords[n-1]);
			}
		}
	}
}










