#include <stdio.h>
#include <math.h>
#include <unistd.h>	
#include <string.h>
#include <ctype.h>
#define ALIVE 'X'
#define DEAD ' '

void setBoards(void);
void printBoard(void);
void printThenBoard(void);
void setScoreBoard(void);
int getScore(int xi, int xj);
void makeChange(int row, int col, int score);
void updateNow(void);
void aChoice(void);
void rChoice(void);
void nChoice(void);

char nowBoard[40][40];
char thenBoard[40][40];
char scoreBoard[42][42];
FILE *input;


int main(int argc, char **argv){

	if (argc > 1){
		input = fopen(argv[1], "r");
	}
	else {
		input = stdin;
	}

	setBoards();
	printBoard();
	char choice;

	while(1){
		fscanf(input, "%c", &choice);
		if(choice == 'q'){
			break;
		}
		switch(choice){
			case 'a':
				aChoice();
				break;
			case 'r':
				rChoice();
				break;
			case 'n':
				nChoice();
				break;
			case 'p':
				while(1){
					usleep(200000);
					nChoice();
				}
			default:
				printBoard();
		}
	}//End While Loop
return(0);
}

void setBoards(void){
	int p, q;
	for( p = 0 ; p < 40 ; p++){
	for( q = 0 ; q < 40 ; q++){
		nowBoard[p][q] = DEAD;
		thenBoard[p][q] = DEAD;
	}
	}
}

void aChoice(void){
	int row, column;
	fscanf(input,"%d", &row);
	fscanf(input,"%d", &column);
	nowBoard[row-1][column-1] = ALIVE;
	printBoard();
}

void rChoice(void){
	int row, column;
	fscanf(input,"%d", &row);
	fscanf(input,"%d", &column);
	nowBoard[row-1][column-1] = ' ';
	printBoard();
}

void nChoice(void){
	int yi, yj;

	setScoreBoard();
	//Now each word Can be checked without worrying about the bounds of nowBoard
	for(yi = 0 ; yi < 40 ; yi++){
	for(yj = 0 ; yj < 40 ; yj++){
		int scoreNum = getScore(yi,yj);
		makeChange( yi , yj , scoreNum );
	}
	}
}

void printBoard(void){
	printf("\033[2J\033[H");
	int i,j;

	printf("\n*  ");
	for(i = 1 ; i < 41 ; i++){
		printf("%-3d", i);
	}
	printf("\n");
	for( i = 0 ; i < 40 ; i++){
		printf("%-3d", i+1);
		for( j = 0 ; j < 40 ; j++){
			printf("%-3c", nowBoard[i][j]);
		}
		printf("\n");
	}

	printf("Please Select An Option:\n");
	printf("a - Add New Live Cell\n");
	printf("r - Remove A Live Cell\n");
	printf("n - Advance The Simulation One Iteration\n");
	printf("q - Quit\n");
	printf("p - Play The Game Forever\n");
}

int getScore(int xi, int xj){
	int l , t;
	int score = 0;

	for(l = 0 ; l < 3 ; l++){
	for(t = 0 ; t < 3 ; t++){
		if(scoreBoard[xi+l][xj+t] == ALIVE){
			score++;
		}
	}
	}

	if(nowBoard[xi][xj] == ALIVE) --score ; 

	return(score);
}

void makeChange(int row, int col, int score){

	char aliveOrDead = nowBoard[row][col];
		
	if(aliveOrDead == ALIVE){
		if(   (score > 3) || (score < 2)   ){
			thenBoard[row][col] = DEAD;
			updateNow();
			printBoard();
		}
		else{
			thenBoard[row][col] = ALIVE;
		}		
	}
	else if(aliveOrDead == DEAD){
		if(score == 3){
			thenBoard[row][col] = ALIVE;
			updateNow();
			printBoard();
		}
		else{
			thenBoard[row][col] = DEAD;
		}
	}
}

void updateNow(void){
	int w, z;
	for( w = 0 ; w < 40 ; w++){
	for( z = 0 ; z < 40 ; z++){
		nowBoard[w][z] = thenBoard[w][z];
	}
	}
}

void setScoreBoard(void){
	int i, j;
	//##################This code is all so that the spaces, including the ones on the edges, will have 8 neighbors.
	//The center 40x40 of scoreBoard will be the same
	for( i = 0 ; i < 40 ; i++){
	for( j = 0 ; j < 40 ; j++){
		scoreBoard[i+1][j+1] = nowBoard[i][j];
		thenBoard[i][j] = nowBoard[i][j];
	}
	}

	//The Outside edges of scoreBoard(except the corners) are the opposite side of nowBoard
	for( i = 0; i < 40 ; i++ ){
		scoreBoard[0][i+1] = nowBoard[39][i];
		scoreBoard[41][i+1] = nowBoard[0][i];
		scoreBoard[i+1][0] = nowBoard[i][39];
		scoreBoard[i+1][41] = nowBoard[i][0];
	}

	//Each Corner is the opposite corner of nowBoar
	scoreBoard[0][0] = nowBoard[39][39];
	scoreBoard[0][41] = nowBoard[39][0];
	scoreBoard[41][41] = nowBoard[0][0];
	scoreBoard[41][0] = nowBoard[0][39];
	//#############################################################################################################
}




