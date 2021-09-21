#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// board information
#define BOARD_SIZE 8
#define EMPTY 0
#define MY_FLAG 2
#define MY_KING 4
#define ENEMY_FLAG 1
#define ENEMY_KING 3


// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define MAX_STEP 15

#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

struct Command
{
	int x[MAX_STEP];
	int y[MAX_STEP];
	int numStep;
	int score;
};

char board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int myFlag;
//���汻���ӣ�besttwostepר�ã�
int secmyeatenflag[MAX_STEP] = { 0 };
int firstmyeatenflag[MAX_STEP] = { 0 };
int enemyeatenflag[MAX_STEP] = { 0 };
//���汻���ӣ�bestonestepר�ã�
int bomyeatenflag[MAX_STEP] = { 0 };
int moveDirBlack[4][2] = { {-1, 1}, {-1, -1}, {1, -1}, {1, 1} }; int moveDirWhite[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
int jumpDir[4][2] = { {2, -2}, {2, 2}, {-2, -2}, {-2, 2} };
int numMyFlag; int numENEMYFlag;
struct Command onemoveCmd = { .x = {0},.y = {0},.numStep = 2,.score = (1 << 31) };
struct Command moveCmd = { .x = {0},.y = {0},.numStep = 2,.score = (1 << 31) };
struct Command jumpCmd = { .x = {0},.y = {0},.numStep = 0 };
struct Command longestJumpCmd = { .x = {0},.y = {0},.numStep = 1 };

struct Command enemymoveCmd = { .x = {0},.y = {0},.numStep = 2 };
struct Command enemyjumpCmd = { .x = {0},.y = {0},.numStep = 0 };
struct Command enemylongestJumpCmd = { .x = {0},.y = {0},.numStep = 1 };

void printBoard()
{
	char visualBoard[BOARD_SIZE][BOARD_SIZE + 1] = { 0 };
	if (myFlag == 1)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				switch (board[i][j])
				{
				case EMPTY:
					visualBoard[i][j] = '.';
					break;
				case ENEMY_FLAG:
					visualBoard[i][j] = 'O';
					break;
				case MY_FLAG:
					visualBoard[i][j] = 'X';
					break;
				case ENEMY_KING:
					visualBoard[i][j] = '@';
					break;
				case MY_KING:
					visualBoard[i][j] = '*';
					break;
				default:
					break;
				}
			}
			printf("%s\n", visualBoard[i]);
		}
	}
	if (myFlag == 2)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				switch (board[i][j])
				{
				case EMPTY:
					visualBoard[i][j] = '.';
					break;
				case ENEMY_FLAG:
					visualBoard[i][j] = 'X';
					break;
				case MY_FLAG:
					visualBoard[i][j] = 'O';
					break;
				case ENEMY_KING:
					visualBoard[i][j] = '*';
					break;
				case MY_KING:
					visualBoard[i][j] = '@';
					break;
				default:
					break;
				}
			}
			printf("%s\n", visualBoard[i]);
		}
	}
}

BOOL isInBound(int x, int y)
{
	return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

int tryToMove(int x, int y)
{
	if (myFlag == 1)
	{
		int newX, newY;
		for (int i = 0; i < board[x][y]; i++)
		{
			newX = x + moveDirBlack[i][0];
			newY = y + moveDirBlack[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				moveCmd.x[0] = x;
				moveCmd.y[0] = y;
				moveCmd.x[1] = newX;
				moveCmd.y[1] = newY;
				return i;
			}
		}
		return -1;
	}
	if (myFlag == 2)
	{
		int newX, newY;
		for (int i = 0; i < board[x][y]; i++)
		{
			newX = x + moveDirWhite[i][0];
			newY = y + moveDirWhite[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				moveCmd.x[0] = x;
				moveCmd.y[0] = y;
				moveCmd.x[1] = newX;
				moveCmd.y[1] = newY;
				return i;
			}
		}
		return -1;
	}
}

int sectryToMove(int x, int y)
{
	if (myFlag == 1)
	{
		int newX, newY;
		for (int i = 0; i < board[x][y]; i++)
		{
			newX = x + moveDirBlack[i][0];
			newY = y + moveDirBlack[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				onemoveCmd.x[0] = x;
				onemoveCmd.y[0] = y;
				onemoveCmd.x[1] = newX;
				onemoveCmd.y[1] = newY;
				return i;
			}
		}
		return -1;
	}
	if (myFlag == 2)
	{
		int newX, newY;
		for (int i = 0; i < board[x][y]; i++)
		{
			newX = x + moveDirWhite[i][0];
			newY = y + moveDirWhite[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				onemoveCmd.x[0] = x;
				onemoveCmd.y[0] = y;
				onemoveCmd.x[1] = newX;
				onemoveCmd.y[1] = newY;
				return i;
			}
		}
		return -1;
	}
}

void tryToJump(int x, int y, int currentStep)
{
	int newX, newY, midX, midY;
	char tmpFlag;
	jumpCmd.x[currentStep] = x;
	jumpCmd.y[currentStep] = y;
	jumpCmd.numStep++;
	for (int i = 0; i < 4; i++)
	{
		newX = x + jumpDir[i][0];
		newY = y + jumpDir[i][1];
		midX = (x + newX) / 2;
		midY = (y + newY) / 2;
		if (isInBound(newX, newY) && (board[midX][midY] & 1) && (board[newX][newY] == EMPTY))
		{
			board[newX][newY] = board[x][y];
			board[x][y] = EMPTY;
			tmpFlag = board[midX][midY];
			board[midX][midY] = EMPTY;
			tryToJump(newX, newY, currentStep + 1);
			board[x][y] = board[newX][newY];
			board[newX][newY] = EMPTY;
			board[midX][midY] = tmpFlag;
		}
	}
	if (jumpCmd.numStep > longestJumpCmd.numStep)
	{
		memcpy(&longestJumpCmd, &jumpCmd, sizeof(struct Command));
	}
	jumpCmd.numStep--;
}

int enemytryToMove(int x, int y)
{
	if (myFlag == 1)
	{
		int newX, newY;
		for (int i = 0; i < board[x][y] + 1; i++)
		{
			newX = x + moveDirWhite[i][0];
			newY = y + moveDirWhite[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				enemymoveCmd.x[0] = x;
				enemymoveCmd.y[0] = y;
				enemymoveCmd.x[1] = newX;
				enemymoveCmd.y[1] = newY;
				return i;
			}
		}
		return -1;
	}
	if (myFlag == 2)
	{
		int newX, newY;
		for (int i = 0; i < board[x][y] + 1; i++)
		{
			newX = x + moveDirBlack[i][0];
			newY = y + moveDirBlack[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				enemymoveCmd.x[0] = x;
				enemymoveCmd.y[0] = y;
				enemymoveCmd.x[1] = newX;
				enemymoveCmd.y[1] = newY;
				return i;
			}
		}
		return -1;
	}
}

void enemytryToJump(int x, int y, int currentStep)
{
	int newX, newY, midX, midY;
	char tmpFlag;
	enemyjumpCmd.x[currentStep] = x;
	enemyjumpCmd.y[currentStep] = y;
	enemyjumpCmd.numStep++;
	for (int i = 0; i < 4; i++)
	{
		newX = x + jumpDir[i][0];
		newY = y + jumpDir[i][1];
		midX = (x + newX) / 2;
		midY = (y + newY) / 2;
		if (isInBound(newX, newY) && ((board[midX][midY] & 1) == 0) && board[midX][midY] > 0 && (board[newX][newY] == EMPTY))
		{
			board[newX][newY] = board[x][y];
			board[x][y] = EMPTY;
			tmpFlag = board[midX][midY];
			board[midX][midY] = EMPTY;
			enemytryToJump(newX, newY, currentStep + 1);
			board[x][y] = board[newX][newY];
			board[newX][newY] = EMPTY;
			board[midX][midY] = tmpFlag;
		}
	}
	if (enemyjumpCmd.numStep > enemylongestJumpCmd.numStep)
	{
		memcpy(&enemylongestJumpCmd, &enemyjumpCmd, sizeof(struct Command));
	}
	enemyjumpCmd.numStep--;
}

struct Command getAllpossiblestep(int x, int y)
{
	if (myFlag == 1)
	{
		struct Command step = {
			.x = {0},
			.y = {0},
			.numStep = 0,
		};
		step.x[0] = x; step.y[0] = y;
		int newX, newY; int num = 1;
		for (int i = 0; i < board[step.x[0]][step.y[0]]; i++)
		{
			newX = step.x[0] + moveDirBlack[i][0];
			newY = step.y[0] + moveDirBlack[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				step.x[num] = newX; step.y[num] = newY; num++;
			}
		}
		step.numStep = num;
		return step;
	}
	if (myFlag == 2)
	{
		struct Command step = {
			.x = {0},
			.y = {0},
			.numStep = 0,
		};
		step.x[0] = x; step.y[0] = y;
		int newX, newY; int num = 1;
		for (int i = 0; i < board[step.x[0]][step.y[0]]; i++)
		{
			newX = step.x[0] + moveDirWhite[i][0];
			newY = step.y[0] + moveDirWhite[i][1];
			if (isInBound(newX, newY) && board[newX][newY] == EMPTY)
			{
				step.x[num] = newX; step.y[num] = newY; num++;
			}
		}
		step.numStep = num;
		return step;
	}
}

void place(struct Command cmd, int object)
{
	if (cmd.numStep == 0)
	{
		return;
	}
	int midX, midY, curFlag;
	curFlag = board[cmd.x[0]][cmd.y[0]];
	for (int i = 0; i < cmd.numStep - 1; i++)
	{
		board[cmd.x[i]][cmd.y[i]] = EMPTY;
		board[cmd.x[i + 1]][cmd.y[i + 1]] = curFlag;
		if (abs(cmd.x[i] - cmd.x[i + 1]) == 2)
		{
			midX = (cmd.x[i] + cmd.x[i + 1]) / 2;
			midY = (cmd.y[i] + cmd.y[i + 1]) / 2;
			if ((board[midX][midY] & 1) == 0)
			{
				numMyFlag--;
			}
			if (object == 1)
			{
				enemyeatenflag[i] = board[midX][midY];
			}
			else if (object == 2)
			{
				firstmyeatenflag[i] = board[midX][midY];
			}
			else if (object == 4)
			{
				secmyeatenflag[i] = board[midX][midY];
			}
			else if (object == 6)
			{
				bomyeatenflag[i] = board[midX][midY];
			}
			board[midX][midY] = EMPTY;
		}
	}
	if (myFlag == 1)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[0][i] == MY_FLAG)
			{
				board[0][i] = MY_KING;
			}
			if (board[BOARD_SIZE - 1][i] == ENEMY_FLAG)
			{
				board[BOARD_SIZE - 1][i] = ENEMY_KING;
			}
		}
	}
	if (myFlag == 2)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			if (board[0][i] == ENEMY_FLAG)
			{
				board[0][i] = ENEMY_KING;
			}
			if (board[BOARD_SIZE - 1][i] == MY_FLAG)
			{
				board[BOARD_SIZE - 1][i] = MY_KING;
			}
		}
	}
}

void recover(struct Command cmd, int oriflag, int object)
{
	if (cmd.numStep == 0)
	{
		return;
	}
	int midX, midY, curFlag;
	curFlag = board[cmd.x[cmd.numStep - 1]][cmd.y[cmd.numStep - 1]];
	for (int i = cmd.numStep - 1; i > 0; i--)
	{
		board[cmd.x[i]][cmd.y[i]] = EMPTY;
		board[cmd.x[i - 1]][cmd.y[i - 1]] = curFlag;
		if (abs(cmd.x[i] - cmd.x[i - 1]) == 2)
		{
			midX = (cmd.x[i] + cmd.x[i - 1]) / 2;
			midY = (cmd.y[i] + cmd.y[i - 1]) / 2;
			numMyFlag++;
			if (object == 1)
			{
				board[midX][midY] = enemyeatenflag[i - 1];
			}
			else if (object == 2)
			{
				board[midX][midY] = firstmyeatenflag[i - 1];
			}
			else if (object == 4)
			{
				board[midX][midY] = secmyeatenflag[i - 1];
			}
			else if (object == 6)
			{
				board[midX][midY] = bomyeatenflag[i - 1];
			}
		}
	}
	if (myFlag == 1)
	{
		if (cmd.x[(cmd.numStep) - 1] == 7)
		{
			if (oriflag == ENEMY_FLAG && curFlag == ENEMY_KING)
			{
				board[cmd.x[0]][cmd.y[0]] = board[cmd.x[0]][cmd.y[0]] - 2;
			}
		}
		else if (cmd.x[(cmd.numStep) - 1] == 0)
		{
			if (oriflag == MY_FLAG && curFlag == MY_KING)
			{
				board[cmd.x[0]][cmd.y[0]] = board[cmd.x[0]][cmd.y[0]] - 2;
			}
		}
	}
	if (myFlag == 2)
	{
		if (cmd.x[(cmd.numStep) - 1] == 0)
		{
			if (oriflag == ENEMY_FLAG && curFlag == ENEMY_KING)
			{
				board[cmd.x[0]][cmd.y[0]] = board[cmd.x[0]][cmd.y[0]] - 2;
			}
		}
		else if (cmd.x[(cmd.numStep) - 1] == 7)
		{
			if (oriflag == MY_FLAG && curFlag == MY_KING)
			{
				board[cmd.x[0]][cmd.y[0]] = board[cmd.x[0]][cmd.y[0]] - 2;
			}
		}
	}
}

void initAI(int me)
{
	numMyFlag = 12; numENEMYFlag = 12;
}

int estimation(const char board[BOARD_SIZE][BOARD_SIZE])
{
	int value = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] == MY_FLAG)
			{
				value = value + 1;
			}
			else if (board[i][j] == MY_KING)
			{
				value = value + 3;
			}
			else if (board[i][j] == ENEMY_FLAG)
			{
				value = value - 1;
			}
			else if (board[i][j] == ENEMY_KING)
			{
				value = value - 3;
			}
		}
	}
	return value;
}


struct Command fakeaiTurn(const char board[BOARD_SIZE][BOARD_SIZE])
{
	struct Command enemycommand =
	{
		.x = {0},
		.y = {0},
		.numStep = 0
	};
	int numChecked = 0;
	int maxStep = 1;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] > 0 && ((board[i][j] == 1) || (board[i][j] == 3)))
			{
				numChecked++;
				enemylongestJumpCmd.numStep = 1;
				enemytryToJump(i, j, 0);
				if (enemylongestJumpCmd.numStep > maxStep)
				{
					memcpy(&enemycommand, &enemylongestJumpCmd, sizeof(struct Command));
					maxStep = enemycommand.numStep;
				}
				if (enemycommand.numStep == 0)
				{
					if (enemytryToMove(i, j) >= 0)
					{
						memcpy(&enemycommand, &enemymoveCmd, sizeof(struct Command));
					}
				}
			}
			if (numChecked >= numENEMYFlag)
			{
				return enemycommand;
			}
		}
	}
	return enemycommand;
}

struct Command onebeststep()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] > 0 && (board[i][j] & 1) == 0)
			{
				struct Command secstep = getAllpossiblestep(i, j);
				for (int pos = 1; pos < secstep.numStep; pos++)
				{
					onemoveCmd.score = (1 << 31);
					int curflag = board[secstep.x[0]][secstep.y[0]];
					board[secstep.x[pos]][secstep.y[pos]] = curflag;
					board[secstep.x[0]][secstep.y[0]] = EMPTY;
					struct Command enemycommand = fakeaiTurn(board);
					int oriflag = board[enemycommand.x[0]][enemycommand.y[0]];
					place(enemycommand, 6);
					int score = estimation(board);
					if (enemycommand.numStep == 0)
					{
						score = ~(1 << 31);
					}
					if (score >= onemoveCmd.score)
					{
						onemoveCmd.x[0] = secstep.x[0];
						onemoveCmd.y[0] = secstep.y[0];
						onemoveCmd.x[1] = secstep.x[pos];
						onemoveCmd.y[1] = secstep.y[pos];
						onemoveCmd.score = score;
					}
					recover(enemycommand, oriflag, 6);
					board[secstep.x[0]][secstep.y[0]] = curflag;
					board[secstep.x[pos]][secstep.y[pos]] = EMPTY;
				}
			}
		}
	}
}

struct Command secaiTurn(const char board[BOARD_SIZE][BOARD_SIZE])
{
	struct Command seccommand =
	{
		.x = {0},
		.y = {0},
		.numStep = 0
	};
	int numChecked = 0; int maxStep = 1;
	onemoveCmd.score = (1 << 31);
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] > 0 && (board[i][j] & 1) == 0)
			{
				numChecked++;
				longestJumpCmd.numStep = 1;
				tryToJump(i, j, 0);
				if (longestJumpCmd.numStep > maxStep)
				{
					memcpy(&seccommand, &longestJumpCmd, sizeof(struct Command));
					maxStep = longestJumpCmd.numStep;
				}
				if (seccommand.numStep == 0)
				{
					if (sectryToMove(i, j) >= 0)
					{
						onebeststep();
						memcpy(&seccommand, &onemoveCmd, sizeof(struct Command));
					}
				}
			}
			if (numChecked >= numMyFlag)
			{
				return seccommand;
			}
		}
	}
	return seccommand;
}

struct Command twobeststep()
{
	moveCmd.score = (1 << 31);
	/*srand((int)time(0));*/
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] > 0 && (board[i][j] & 1) == 0)
			{
				struct Command step = getAllpossiblestep(i, j);
				for (int pos = 1; pos < step.numStep; pos++)
				{
					int score = 0; int skipsign = 0;
					int curflag = board[step.x[0]][step.y[0]];
					board[step.x[pos]][step.y[pos]] = curflag;
					board[step.x[0]][step.y[0]] = EMPTY;
					struct Command enemycommand = fakeaiTurn(board);
					if (enemycommand.numStep == 0)
					{
						score = ~(1 << 31);
						skipsign = 1;
						goto skip;
					}
					int oriflag = 0;
					oriflag = board[enemycommand.x[0]][enemycommand.y[0]];
					place(enemycommand, 2);
					struct Command seccommand = secaiTurn(board);
					int selforiflag = 0;
					selforiflag = board[seccommand.x[0]][seccommand.y[0]];
					place(seccommand, 1);
					struct Command secenemycommand = fakeaiTurn(board);
					int secoriflag = 0;
					secoriflag = board[secenemycommand.x[0]][secenemycommand.y[0]];
					place(secenemycommand, 4);
					score = estimation(board);
					if (secenemycommand.numStep == 0)
					{
						score = ~(1 << 31);
					}
				skip:
					if (score > moveCmd.score)
					{
						moveCmd.x[0] = step.x[0];
						moveCmd.y[0] = step.y[0];
						moveCmd.x[1] = step.x[pos];
						moveCmd.y[1] = step.y[pos];
						moveCmd.score = score;
						if (skipsign == 1)
						{
							board[step.x[0]][step.y[0]] = curflag;
							board[step.x[pos]][step.y[pos]] = EMPTY;
							return;
						}
					}
					recover(secenemycommand, secoriflag, 4);
					recover(seccommand, selforiflag, 1);
					recover(enemycommand, oriflag, 2);
					board[step.x[0]][step.y[0]] = curflag;
					board[step.x[pos]][step.y[pos]] = EMPTY;
				}
			}
		}
	}
}

struct Command aiTurn(const char board[BOARD_SIZE][BOARD_SIZE], int me)
{
	struct Command command =
	{
		.x = {0},
		.y = {0},
		.numStep = 0
	};
	int numChecked = 0; int maxStep = 1;
	moveCmd.score = (1 << 31);
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] > 0 && (board[i][j] & 1) == 0)
			{
				numChecked++;
				longestJumpCmd.numStep = 1;
				tryToJump(i, j, 0);
				if (longestJumpCmd.numStep > maxStep)
				{
					memcpy(&command, &longestJumpCmd, sizeof(struct Command));
					maxStep = longestJumpCmd.numStep;
				}
				if (command.numStep == 0)
				{
					if (tryToMove(i, j) >= 0)
					{
						twobeststep();
						memcpy(&command, &moveCmd, sizeof(struct Command));
					}
				}
			}
			if (numChecked >= numMyFlag)
			{
				return command;
			}
		}
	}
	return command;
}

void start(int myFlag)
{
	memset(board, 0, sizeof(board));
	if (myFlag == 1)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 8; j += 2)
			{
				board[i][j + (i + 1) % 2] = ENEMY_FLAG;
			}
		}
		for (int i = 5; i < 8; i++)
		{
			for (int j = 0; j < 8; j += 2)
			{
				board[i][j + (i + 1) % 2] = MY_FLAG;
			}
		}
	}
	if (myFlag == 2)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 8; j += 2)
			{
				board[i][j + (i + 1) % 2] = MY_FLAG;
			}
		}
		for (int i = 5; i < 8; i++)
		{
			for (int j = 0; j < 8; j += 2)
			{
				board[i][j + (i + 1) % 2] = ENEMY_FLAG;
			}
		}
	}
	initAI(myFlag);
}

void turn()
{
	struct Command command = aiTurn((const char(*)[BOARD_SIZE])board, myFlag);
	place(command, 1);
	printf("%d", command.numStep);
	for (int i = 0; i < command.numStep; i++)
	{
		printf(" %d,%d", command.x[i], command.y[i]);
	}
	printf("\n");
	fflush(stdout);
}

void end(int x)
{
	exit(0);
}

void loop()
{
	char tag[10] = { 0 };
	struct Command command =
	{
		.x = {0},
		.y = {0},
		.numStep = 0
	};
	int status;
	while (TRUE)
	{
		memset(tag, 0, sizeof(tag));
		scanf_s("%s", tag, sizeof(tag));
		if (strcmp(tag, START) == 0)
		{
			scanf_s("%d", &myFlag);
			start(myFlag);
			printf("OK\n");
			fflush(stdout);
		}
		else if (strcmp(tag, PLACE) == 0)
		{
			scanf_s("%d", &command.numStep);
			for (int i = 0; i < command.numStep; i++)
			{
				scanf_s("%d,%d", &command.x[i], &command.y[i]);
			}
			place(command, 2);
		}
		else if (strcmp(tag, TURN) == 0)
		{
			turn();
		}
		else if (strcmp(tag, END) == 0)
		{
			scanf_s("%d", &status);
			end(status);
		}
		printBoard();
	}
}

int main(int argc, char* argv[])
{
	loop();
	return 0;
}