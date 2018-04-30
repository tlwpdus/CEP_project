#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define MAXN		13
#define MAXM		20
#define SQUARE_NUM		(MAXN*MAXM)
#define SHUFFLE_NUM		1000000

int n, m, k;

int mine_board[MAXN][MAXM];
/* 0 : non-mined, 1 : mined */
int flag_board[MAXN][MAXM];
/* 0 : no-flag, 1 : flag*/
int display_board[MAXN][MAXM];
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
void err();
int input();
void endgame();
void init_setting();
int newgame();

int main(void) {
	while(1) {
		init_setting();
		init_board();
		while(!check_game()) {
			show_interface();
			intro();
			if (input()==1) break;
		}
		endgame();
		if (!newgame()) break;
	}
}

void init_setting() {
	int nen[] = {5,10,10,13,13};
	int men[] = {5,10,15,15,20};
	int ken[] = {5,12,19,30,41};
	while(1) {
		system("clear");
		printf("Select Difficulty(1~5) : ");
		int df;
		scanf("%d",&df);
		if (df<=0||df>5) {
			err();
			continue;
		}
		df--;
		n=nen[df];m=men[df];k=ken[df];
		break;
	}
}

int newgame() {
	while(1) {
		printf("Again? (Y/n) : ");
		char ch;
		scanf(" %c",&ch);
		if (ch=='Y'||ch=='y') return 1;
		else if (ch=='N'||ch=='n') return 0;
		else err();
	}
	return 0;
}

void endgame() {
	show_interface();
	if (check_game()) printf("YOU WON!\n");
	else printf("YOU LOST!\n");
}

void err() {
	printf("\n");
	printf("*----------------------------*\n");
	printf("|    Enter valid command!    |\n");
	printf("| Press enter to continue... |\n");
	printf("*----------------------------*\n\n");
	scanf("%*c%*c");
}

int input() {
	int x, y;
	scanf("%d",&y); y--;
	if (y<0) {
		scanf("%d%d",&y,&x); x--; y--;
		if (!ok(x,y)||display_board[x][y]) {
			err(); return -1;
		}
		flag_board[x][y] ^= 1;
	}
	else {
		scanf("%d",&x); x--;
		if (!ok(x,y)) {
			err(); return -1;
		}
		if (sweep(x,y)) return 1;
	}
	return 0;
}

void intro() {
	printf("By entering \"x y\", you can simply check the location (x,y)\n");
	printf("By entering \"0 x y\", you can toggle flag at (x,y)\n\n");
	printf("Enter command : ");
}

void init_board() {
	int i;
	int shuffle[MAXN*MAXM];
	int temp;
	int r1, r2;

	memset(mine_board,0,sizeof(mine_board));
	memset(display_board,0,sizeof(display_board));
	memset(flag_board,0,sizeof(flag_board));

	srand(time(NULL)); // set seed

	// initialize shuffle array
	for(i=0; i<n*m; i++)
		shuffle[i] = i;

	// shuffling
	for(i=0; i<n*m; i++) {
		r1 = rand() % (n*m);
		r2 = rand() % (n*m);
		temp = shuffle[r1];
		shuffle[r1] = shuffle[r2];
		shuffle[r2] = temp;
	}

	// get mine coordinates from shuffled array
	for(i=0; i<k; i++)
		mine_board[shuffle[i]/m][shuffle[i]%m] = 1;
}

void show_interface() {
	int i, j;

	system("clear"); // clear the screen
	// rest of this function just prints out display_board
	printf("    ");
	for(i=0; i<m; i++)
		printf(" %2d ", i+1);
	printf("-> X\n");

	for(i=0; i<n; i++) {
		printf("\n %2d ", i+1);
		for(j=0; j<m; j++) {
			if(display_board[i][j] == -1) {
				if(mine_board[i][j] == 1)
					printf("  * ");
				else
					printf("  X ");
			}
			else if(display_board[i][j] == 0) {
				if (flag_board[i][j]==1)
					printf("  ! ");	
				else printf("  - ");
			}
			else
			printf("  %d ", display_board[i][j]);
		}
		printf("\n");
	}
	printf("\n  |\n  v\n\n  Y\n\n");
}

int ok(int x, int y) {
	return x>=0&&x<n&&y>=0&&y<m;
}

int px[] = {-1,-1,-1,0,0,1,1,1};
int py[] = {-1,0,1,-1,1,-1,0,1};
int sweep(int x, int y) {
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

int check_game() {
	int i, j;
	for (i=0;i<n;i++) {
		for (j=0;j<m;j++) {
			if (display_board[i][j]==0&&mine_board[i][j]==0) return 0;
		}
	}
	return 1;
}
