#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BOARD_SIZE		10
#define SQUARE_NUM		(BOARD_SIZE*BOARD_SIZE)
#define MINE_NUM		12
#define SHUFFLE_NUM		100000

int mine_board[BOARD_SIZE][BOARD_SIZE];
/* 0 : non-mined, 1 : mined */
int display_board[BOARD_SIZE][BOARD_SIZE];
/* -1 : no mines around, 0 : unknown, 1~8 : number of mines */

void init_board(); // initialize mine_board by randomly planting fixed number of mines
void show_interface(); // print display_board
int sweep(int x, int y);
/*
 * sweep a square on (x, y). if there is no mines around square on (x, y), recursively sweep adjacent squares
 * return : 1 if player sweeps mined square, else 0
 */
int check_game(); 
/*
 * check if the player swept all non-mined squares
 * return : 1 if player swept all non-mined squares, else 0
 */
int ok(int x, int y);
void intro();

int main(void)
{
	init_board();
	while(!check_game()) {
		show_interface();
		intro();
		int x, y;
		scanf("%d%d",&y,&x); x--; y--;
		if (!ok(x,y)) {
			printf("Enter valid location!\n");
			printf("Press enter to continue...\n");
			scanf("%*c%*c");
			continue;
		}
		if (sweep(x,y)) break;
	}
	show_interface();
	if (check_game()) printf("YOU WON!\n");
	else printf("YOU LOST!\n");
}

void intro() {
	printf("By entering \"x y\", you can simply check the location (x,y)\n");
	printf("By entering \"0 x y\", you can toggle flag at (x,y)\n\n");
	printf("Enter command : ");
}

void init_board()
{
	int i;
	int shuffle[BOARD_SIZE * BOARD_SIZE];
	int temp;
	int r1, r2;

	srand(time(NULL)); // set seed

	// initialize shuffle array
	for(i=0; i<SQUARE_NUM; i++)
		shuffle[i] = i;

	// shuffling
	for(i=0; i<SHUFFLE_NUM; i++)
	{
		r1 = rand() % SQUARE_NUM;
		r2 = rand() % SQUARE_NUM;

		temp = shuffle[r1];
		shuffle[r1] = shuffle[r2];
		shuffle[r2] = temp;
	}

	// get mine coordinates from shuffled array
	for(i=0; i<MINE_NUM; i++)
		mine_board[shuffle[i]/BOARD_SIZE][shuffle[i]%BOARD_SIZE] = 1;
}

void show_interface()
{
	int i, j;

	system("clear"); // clear the screen
	// rest of this function just prints out display_board
	printf("    ");
	for(i=0; i<BOARD_SIZE; i++)
		printf(" %2d ", i+1);
	printf("-> X\n");

	for(i=0; i<BOARD_SIZE; i++)
	{
		printf("\n %2d ", i+1);

		for(j=0; j<BOARD_SIZE; j++)
		{
			if(display_board[i][j] == -1)
			{
				if(mine_board[i][j] == 1)
					printf("  * ");
				else
					printf("  X ");
			}
			else if(display_board[i][j] == 0) {
				printf("  - ");
			}
			else
				printf("  %d ", display_board[i][j]);
		}
		printf("\n");
	}
	printf("\n  |\n  v\n\n  Y\n\n");
}

int ok(int x, int y) {
	return x>=0&&x<BOARD_SIZE&&y>=0&&y<BOARD_SIZE;
}

int px[] = {-1,-1,-1,0,0,1,1,1};
int py[] = {-1,0,1,-1,1,-1,0,1};
int sweep(int x, int y)
{
	if (display_board[x][y]) return 0;
	if (mine_board[x][y]==1) {
		display_board[x][y] = -1;
		return 1;
	}
	int i, cnt = 0;
	for (i=0;i<8;i++) {
		int nx = x+px[i], ny=y+py[i];
		if (!ok(nx,ny)) continue;
		if (mine_board[nx][ny]) cnt++;
	}
	if (!cnt) {
		display_board[x][y] = -1;
		for (i=0;i<8;i++) {
			int nx = x+px[i], ny=y+py[i];
			if (!ok(nx,ny)) continue;
			sweep(nx,ny);
		}
		return 0;
	}
	display_board[x][y] = cnt;
	return 0;
}

int check_game()
{
	int i, j;
	for (i=0;i<BOARD_SIZE;i++) {
		for (j=0;j<BOARD_SIZE;j++) {
			if (display_board[i][j]==0&&mine_board[i][j]==0) return 0;
		}
	}
	return 1;
}
