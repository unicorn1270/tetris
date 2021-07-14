//Name: configuration.cpp
//Description: Define global variables, functions
/*Parameters Setting*/

/*include header files*/

#include "headerfileset.h"
#include "configuration.h"
#include "main.h"

//test point
#define IFRUN 1

///*Interface Setting*/
//
//#define WINDOWINTERFACE_SIZE_LENGTH 40
//#define WINDOWINTERFACE_SIZE_HIGHT 30
//#define GAMEINTERFACE_SIZE_LENGTH 30
//#define GAMEINTERFACE_SIZE_HIGHT 30
//#define GAME_INTF_X 25//game interface position X
//#define GAME_INTF_Y 30//game interface position Y
//
///*Function Key*/
//
//#define DIR_LEFT 75
//#define DIR_RIGHT 77
//#define DIR_DOWN 80
//#define SPACE 32
//#define ESC 27
//
///*Game Parameters Setting*/
//
//#define BLANK 0
//#define BLOCK 1
//#define WALL 2
//#define BASEBLOCKS 7//seven basic blocks
//#define BLOCKS_SIZE 4//size of block: 4*4
//#define RTTSTATES 4//four rotating states

/*Set Gobal Variables*/

int g_score = 0;
int g_game_level = 0;
int g_block_basicstate = 0;
int g_fallspeed = 15000;

/*Set Structures*/

//set window interface parameters
struct WINDOW_INTF window_intf_t[WINDOWINTERFACE_SIZE_LENGTH][WINDOWINTERFACE_SIZE_HIGHT];
//set blocks parameters
struct BLOCKS blocks_t[BASEBLOCKS][BLOCKS_SIZE];
//store temp block's position(when "stop")
struct TEMP_BLOCK temp_block_t;

//define external call structures
extern struct WINDOW_INTF window_intf_t[WINDOWINTERFACE_SIZE_LENGTH][WINDOWINTERFACE_SIZE_HIGHT];
extern struct BLOCKS blocks_t[BASEBLOCKS][BLOCKS_SIZE];
extern struct TEMP_BLOCK temp_block_t;

/*Set Functions*/

//Name: HideCursor(void)
//Input Type: int * 1
//Return Type: void
//Description: Hide the cursor of the window
void HideCursor(int mode)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = mode;//when "cci.bVisible" = 0,cursor will be hidden
	SetConsoleCursorInfo(hOut, &cci);
}

//Name: ChangeColor(int colorchioce)
//Input Type: int * 1
//Return Type: void
//Description: Change the color of the output text
void ChangeColor(int colorchioce)
{
	switch (colorchioce)
	{
	case 0: colorchioce = 8; break;
	case 1: colorchioce = 3; break;
	case 2: colorchioce = 10; break;
	case 3: colorchioce = 11; break;
	case 4: colorchioce = 12; break;
	case 5: colorchioce = 13; break;
	case 6: colorchioce = 14; break;
	case 7: colorchioce = 7; break;
	default: colorchioce = 7; break;
	}
	//API function
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorchioce);
}

//Name: GotoPosition(int position_x, int position_y)
//Input Type: int * 2
//Return Type: void
//Description: Goto the position (x,y)
void GotoPosition(int position_x, int position_y)
{
	COORD winintf_position;
	winintf_position.X = position_x;
	winintf_position.Y = position_y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), winintf_position);
}

//Name: InitBlocks(void)
//Input Type: void
//Return Type: void
//Description: Initialize blocks' information
void InitBlocks(void)
{
	/*seven based blocks*/

	/*inverted "土" shape*/
	blocks_t[0][0].formation[1][0] = BLOCK;
	blocks_t[0][0].formation[1][1] = BLOCK;
	blocks_t[0][0].formation[1][2] = BLOCK;
	blocks_t[0][0].formation[2][1] = BLOCK;
	/*"L" shape1*/
	blocks_t[1][0].formation[1][1] = BLOCK;
	blocks_t[1][0].formation[2][1] = BLOCK;
	blocks_t[1][0].formation[3][1] = BLOCK;
	blocks_t[1][0].formation[1][2] = BLOCK;
	/*"L" shape2*/
	blocks_t[2][0].formation[1][2] = BLOCK;
	blocks_t[2][0].formation[2][2] = BLOCK;
	blocks_t[2][0].formation[3][2] = BLOCK;
	blocks_t[2][0].formation[1][1] = BLOCK;
	/*"Z" shape1*/
	blocks_t[3][0].formation[1][0] = BLOCK;
	blocks_t[3][0].formation[1][1] = BLOCK;
	blocks_t[3][0].formation[2][1] = BLOCK;
	blocks_t[3][0].formation[2][2] = BLOCK;
	/*"Z" shape2*/
	blocks_t[4][0].formation[1][1] = BLOCK;
	blocks_t[4][0].formation[1][2] = BLOCK;
	blocks_t[4][0].formation[2][0] = BLOCK;
	blocks_t[4][0].formation[2][1] = BLOCK;
	/*"田" shape*/
	blocks_t[5][0].formation[1][1] = BLOCK;
	blocks_t[5][0].formation[1][2] = BLOCK;
	blocks_t[5][0].formation[2][1] = BLOCK;
	blocks_t[5][0].formation[2][2] = BLOCK;
	/*"1" shape*/
	blocks_t[6][0].formation[0][2] = BLOCK;
	blocks_t[6][0].formation[1][2] = BLOCK;
	blocks_t[6][0].formation[2][2] = BLOCK;
	blocks_t[6][0].formation[3][2] = BLOCK;

	/*twenty-eight(4 * 7 = 28) rotating states*/
	int base, rttstate, index1, index2, tmp[BLOCKS_SIZE][BLOCKS_SIZE] = { 0 };

	for (base = 0; base < 7; base++)
	{
		for (rttstate = 0; rttstate < 3; rttstate++)
		{
			for (index1 = 0; index1 < 4; index1++)
			{
				for (index2 = 0; index2 < 4; index2++)
				{
					tmp[index1][index2] = blocks_t[base][rttstate].formation[index1][index2];
				}
			}
			for (index1 = 0; index1 < 4; index1++)
			{
				for (index2 = 0; index2 < 4; index2++)
				{
					blocks_t[base][rttstate + 1].formation[index1][index2] = tmp[4 - index2 - 1][index1];
				}
			}
		}
	}
}

//Name: DrawBlank(int base, int rttstate, int position_x, int position_y)
//Input Type: int * 4
//Return Type: void
//Description: Cover the box of the previous blocks and draw empty instead
void DrawBlank(int base, int rttstate, int position_x, int position_y)
{
	static int index1 = 0, index2 = 0;

	for (index1 = 0; index1 < BLOCKS_SIZE; index1++)
	{
		for (index2 = 0; index2 < BLOCKS_SIZE; index2++)
		{
			GotoPosition(2 * (position_x + index2), position_y + index1);
			if (blocks_t[base][rttstate].formation[index1][index2] == BLOCK)
				printf("  ");
		}
	}
}

//Name: DrawBlock(int base, int rttstate, int position_x, int position_y)
//Input Type: int * 4
//Return Type: void
//Description: Draw the current block
void DrawBlock(int base, int rttstate, int position_x, int position_y)
{
	static int index1 = 0, index2 = 0;

	for (index1 = 0; index1 < BLOCKS_SIZE; index1++)
	{
		for (index2 = 0; index2 < BLOCKS_SIZE; index2++)
		{
			GotoPosition(2 * (position_x + index2), position_y + index1);
			if (blocks_t[base][rttstate].formation[index1][index2] == BLOCK)
				printf("■");
		}
	}
}

//Name: IsBlocksHit(int base, int rttstate, int position_x, int position_y)
//Input Type: int * 4
//Return Type: bool * 1
//Description: Determine whether a block falling or moving will collide with a wall or other blocks
bool IsBlocksHit(int base, int rttstate, int position_x, int position_y)
{
	static int index1 = 0, index2 = 0;

	for (index1 = 0; index1 < BLOCKS_SIZE; index1++)
	{
		for (index2 = 0; index2 < BLOCKS_SIZE; index2++)
		{
			if (blocks_t[base][rttstate].formation[index1][index2] == 0)
				continue;
			else if (window_intf_t[position_x + index2][position_y + index1].state == WALL || window_intf_t[position_x + index2][position_y + index1].state == BLOCK)
				return TRUE;
		}
	}
	return FALSE;
}

//Name: CreateWinIntf(void)
//Input Type: void
//Return Type: void
//Description: Create the window interface(title,size)
void CreateWinIntf(void)
{
	system("title Tetris Game");
	system("mode con lines=30 cols=80");
}

//Name: InitWindowIntf(void)
//Input Type: void
//Return Type: void
//Description: Initialize the window(game) interface(size,type)
void InitWindowIntf(void)
{
	static int index1 = 0, index2 = 0;

	ChangeColor(7);

	for (index1 = 0; index1 < WINDOWINTERFACE_SIZE_HIGHT; index1++)
	{
		for (index2 = 0; index2 < WINDOWINTERFACE_SIZE_LENGTH; index2++)
		{
			if (index2 == 0 || index2 == GAMEINTERFACE_SIZE_LENGTH - 1 || index2 == WINDOWINTERFACE_SIZE_LENGTH - 1)
			{
				window_intf_t[index2][index1].state = WALL;
				GotoPosition(2 * index2, index1);
				printf("■");
			}
			else if (index1 == WINDOWINTERFACE_SIZE_HIGHT - 1)
			{
				window_intf_t[index2][index1].state = WALL;
				printf("■");
			}
			else
				window_intf_t[index2][index1].state = BLANK;
		}
	}

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 19);
	printf("左移: ←");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 17);
	printf("右移: →");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 15);
	printf("旋转: SPACE");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 13);
	printf("暂停: S");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 11);
	printf("退出: ESC");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 9);
	printf("重新开始: R");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 5);
	printf("分数: %d", g_score);

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 3);
	printf("等级: %d", g_game_level);
}

//Name: GetChoice(int base, int rttstate)
//Input Type: int* * 2
//Return Type: void
//Description: Get user's choice(function keys,direction keys)
void GetChoice(int* base, int* rttstate, int* position_x, int* position_y)
{
	static int input = 0;

	input = getch();
	switch (input)
	{
	case DIR_LEFT:
		if (IsBlocksHit(*base, *rttstate, *position_x - 1, *position_y) != TRUE)
		{
			DrawBlank(*base, *rttstate, *position_x, *position_y);
			(*position_x)--;
		}
		break;
	case DIR_RIGHT:
		if (IsBlocksHit(*base, *rttstate, *position_x + 1, *position_y) != TRUE)
		{
			DrawBlank(*base, *rttstate, *position_x, *position_y);
			(*position_x)++;
		}
		break;
	case DIR_DOWN:
		if (IsBlocksHit(*base, *rttstate, *position_x, *position_y + 1) != TRUE)
		{
			DrawBlank(*base, *rttstate, *position_x, *position_y);
			(*position_y)++;
		}
		break;
	case SPACE:
		if (IsBlocksHit(*base, (*rttstate + 1) % 4, *position_x, *position_y) != TRUE)
		{
			DrawBlank(*base, *rttstate, *position_x, *position_y);
			*rttstate = (*rttstate + 1) % 4;
		}
		break;
	case ESC:
		ESC_QuitGame();
		break;
	case 'S':
	case 's':
		PauseGame(*base, *rttstate, *position_x, *position_y);
		break;
	case 'R':
	case 'r':
		RestartGame(*base, *rttstate, *position_x, *position_y);
		break;
	}
}

//Name: StartGame(void)
//Input Type: void
//Return Type: void
//Description: The progress of the game(random blocks,blocks preview,clear blocks,getchoice,scorecnt)
void StartGame(void)
{
	int rttstate = 0, temp_block_basestate = 0, 
		block_initPosition_x = GAMEINTERFACE_SIZE_LENGTH / 2 - 2, block_initPosition_y = 0;
	int tempblock_initPosition_x = block_initPosition_x, tempblock_initPosition_y = block_initPosition_y;
	int tempfallspeed = 0, index1, index2;

	DrawBlank(g_block_basicstate, rttstate, WINDOWINTERFACE_SIZE_LENGTH - 10, 4);

	temp_block_basestate = g_block_basicstate;
	g_block_basicstate = rand() % BASEBLOCKS;

	ChangeColor(g_block_basicstate);
	DrawBlock(g_block_basicstate, rttstate, WINDOWINTERFACE_SIZE_LENGTH - 10, 4);

	while (IFRUN)
	{
		ChangeColor(temp_block_basestate);
		DrawBlock(temp_block_basestate, rttstate, tempblock_initPosition_x, tempblock_initPosition_y);
		if (tempfallspeed == 0)
			tempfallspeed = g_fallspeed;

		while (--tempfallspeed)
		{
			if (kbhit() != 0)
				break;
		}

		if (tempfallspeed == 0)
		{
			if (IsBlocksHit(temp_block_basestate, rttstate, tempblock_initPosition_x, tempblock_initPosition_y + 1) != TRUE)
			{
				DrawBlank(temp_block_basestate, rttstate, tempblock_initPosition_x, tempblock_initPosition_y);
				tempblock_initPosition_y++;
			}
			else
			{
				PlayBlockHitMusic();

				for (index1 = 0; index1 < BLOCKS_SIZE; index1++)
				{
					for (index2 = 0; index2 < BLOCKS_SIZE; index2++)
					{
						if (blocks_t[temp_block_basestate][rttstate].formation[index1][index2] == BLOCK)
						{
							window_intf_t[tempblock_initPosition_x + index2][tempblock_initPosition_y + index1].state = BLOCK;
							window_intf_t[tempblock_initPosition_x + index2][tempblock_initPosition_y + index1].color = temp_block_basestate;
							while (ClearBlocks());
						}
					}
				}
				return;
			}
		}

		else
		{
			GetChoice(&temp_block_basestate, &rttstate, &tempblock_initPosition_x, &tempblock_initPosition_y);
		}

	}
}

//Name: ClearBlocks(void)
//Input Type: void
//Return Type: bool * 1
//Description: Judge whether there is a full line,clear the full line and calculate score and level
bool ClearBlocks(void)
{
	int index1, index2, blockcnt, overwrite_index1, overwrite_index2;
		
	for (index1 = GAME_INTF_Y - 2; index1 > 4; index1--)
	{
		blockcnt = 0;

		for (index2 = 1; index2 < GAME_INTF_X - 1; index2++)
		{
			blockcnt += window_intf_t[index2][index1].state;
		}
		if (blockcnt == 0)
			break;
		if (blockcnt == GAME_INTF_X - 2)
		{
			GameLevelCnt();
			ChangeColor(7);
			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 5);
			printf("分数: %d", g_score);
			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 3);
			printf("等级: %d", g_game_level);

			for (index2 = 1; index2 < GAME_INTF_X - 1; index2++)
			{
				window_intf_t[index2][index1].state = BLANK;
				GotoPosition(2 * index2, index1);
				printf("  ");
			}

			for (overwrite_index1 = index1; overwrite_index1 > 0; overwrite_index1--)
			{
				blockcnt = 0;
				for (overwrite_index2 = 1; overwrite_index2 < GAME_INTF_X - 1; overwrite_index2++)
				{
					blockcnt += window_intf_t[overwrite_index2][overwrite_index1 - 1].state;
					window_intf_t[overwrite_index2][overwrite_index1].state = window_intf_t[overwrite_index2][overwrite_index1 - 1].state;
					window_intf_t[overwrite_index2][overwrite_index1].color = window_intf_t[overwrite_index2][overwrite_index1 - 1].color;
					if (window_intf_t[overwrite_index2][overwrite_index1].state == BLANK)
					{
						GotoPosition(2 * overwrite_index2, overwrite_index1);
						printf("  ");
					}
					else
					{
						GotoPosition(2 * overwrite_index2, overwrite_index1);
						ChangeColor(window_intf_t[overwrite_index2][overwrite_index1].color);
						printf("■");
					}
				}
				if (blockcnt == 0)
					return TRUE;
			}
		}
	}

	for (index2 = 1; index2 < GAME_INTF_X - 1; index2++)
	{
		if (window_intf_t[index2][index1].state == BLOCK)
		{
			PlayBGM(0);
			GameFailure();
		}
	}
	return FALSE;
}

//Name: GameLoop(void)
//Input Type: void
//Return Type: void
//Description: Integrating game initialization and game process
void GameLoop(void)
{
	PlayBGM(1);
	InitWindowIntf();
	InitBlocks();
	SetInitialBlock();
	while (IFRUN)
	{
		StartGame();
	}
}

//Name: SetInitialBlock(void)
//Input Type: void
//Return Type: void
//Description: Set random seed, set the first block
void SetInitialBlock(void)
{
	srand((unsigned int)time(NULL));
	g_block_basicstate = rand() % BASEBLOCKS;
}

//Name: PlayBGM(int mode)
//Input Type: int * 1
//Return Type: void
//Description: Open the file of background music,and switch play mode to play background music
void PlayBGM(int mode)
{
	mciSendString(TEXT("open backg1.mp3 Alias backgroundmusic"), NULL, 0, NULL);
	
	if (mode == 0)
	    mciSendString(TEXT("stop backgroundmusic"), NULL, 0, NULL);
	if (mode == 1)
		mciSendString(TEXT("play backgroundmusic repeat"), NULL, 0, NULL);
	if (mode == 2)
		mciSendString(TEXT("pause backgroundmusic"), NULL, 0, NULL);
}

//Name: PlayBlockHitMusic(void)
//Input Type: void
//Return Type: void
//Description: Play blockhitmusic when blocks hit
void PlayBlockHitMusic(void)
{
	mciSendString(TEXT("open blockhit.mp3 Alias blockhitmusic"), NULL, 0, NULL);
	mciSendString(TEXT("seek blockhitmusic to start"), NULL, 0, NULL);
	mciSendString(TEXT("play blockhitmusic"), NULL, 0, NULL);
}

//Name: GameLevelCnt(void)
//Input Type: void
//Return Type: void
//Description: Calculate game score and game level, adjust the speed of blocks' falling
//every level will speed up 200 times(from keyboard)
void GameLevelCnt(void)
{
	static int MAXLEVEL = 20;

	g_score += 100;
	if (g_game_level < MAXLEVEL)
	{
		g_game_level++;
		g_fallspeed -= g_game_level * 200;
	}
}

//Name: LoadDisplay(void)
//Input Type: void
//Return Type: void
//Description: Show the game loading screen and prepare for starting game
void LoadDisplay(void)
{
	static int character_TETRIS[WINDOWINTERFACE_SIZE_LENGTH][WINDOWINTERFACE_SIZE_HIGHT] = { 0 };
	static int index_posX = 0, index_posY = 0;


	//character 'T'
	for (index_posX = 4; index_posX <= 9; index_posX++)
	{
		for (index_posY = 11; index_posY <= 12; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 6; index_posX <= 7; index_posX++)
	{
		for (index_posY = 13; index_posY <= 18; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	//charater 'E'
	for (index_posX = 10; index_posX <= 13; index_posX++)
	{
		for (index_posY = 11; index_posY <= 12; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 10; index_posX <= 13; index_posX++)
	{
		for (index_posY = 17; index_posY <= 18; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 10, index_posY = 13; index_posY <= 16; index_posY++)
	{
		character_TETRIS[index_posX][index_posY] = BLOCK;
	}

	for (index_posX = 11; index_posX <= 12; index_posX++)
	{
		for (index_posY = 14; index_posY <= 15; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	//character 'T'
	for (index_posX = 14; index_posX <= 19; index_posX++)
	{
		for (index_posY = 11; index_posY <= 12; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 16; index_posX <= 17; index_posX++)
	{
		for (index_posY = 13; index_posY <= 18; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	//character 'R'
	for (index_posX = 20; index_posX <= 24; index_posX++)
	{
		for (index_posY = 11; index_posY <= 12; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 20; index_posX <= 21; index_posX++)
	{
		for (index_posY = 13; index_posY <= 18; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 24, index_posY = 13; index_posY <= 14; index_posY++)
	{
		character_TETRIS[index_posX][index_posY] = BLOCK;
	}

	for (index_posY = 15, index_posX = 22; index_posX <= 23; index_posX++)
	{
		character_TETRIS[index_posX][index_posY] = BLOCK;
	}

	character_TETRIS[23][16] = BLOCK;

	for (index_posX = 24, index_posY = 17; index_posY <= 18; index_posY++)
	{
		character_TETRIS[index_posX][index_posY] = BLOCK;
	}

	character_TETRIS[25][18] = BLOCK;

	//character 'I'
	for (index_posX = 27; index_posX <= 28; index_posX++)
	{
		for (index_posY = 11; index_posY <= 18; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	//character 'S'
	for (index_posX = 32; index_posX <= 35; index_posX++)
	{
		for (index_posY = 11; index_posY <= 12; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 31; index_posX <= 32; index_posX++)
	{
		for (index_posY = 12; index_posY <= 14; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 32; index_posX <= 34; index_posX++)
	{
		for (index_posY = 14; index_posY <= 15; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 34; index_posX <= 35; index_posX++)
	{
		for (index_posY = 15; index_posY <= 17; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	for (index_posX = 31; index_posX <= 34; index_posX++)
	{
		for (index_posY = 17; index_posY <= 18; index_posY++)
		{
			character_TETRIS[index_posX][index_posY] = BLOCK;
		}
	}

	//display characters
	ChangeColor(7);
	for (index_posY = 11; index_posY <= 18; index_posY++)
	{
		for (index_posX = 4; index_posX <= 35; index_posX++)
		{
			if (character_TETRIS[index_posX][index_posY] == BLOCK)
			{
				GotoPosition(2 * index_posX, index_posY);
				printf("■");
			}
		}
		Sleep(500);
	}
	Sleep(2000);
	system("cls");
}

//Name: PauseGame(int basestate, int rttstate, int block_posX, int block_posY)
//Input Type: int * 4
//Return Type: void
//Description: Store the positions of game window interface and block when pause the game
void PauseGame(int basestate, int rttstate, int block_posX, int block_posY)
{
	static int index_posX = 0, index_posY = 0;
	static int index1 = 0, index2 = 0;

	temp_block_t.position_x = block_posX;
	temp_block_t.position_y = block_posY;

	PlayBGM(2);
	system("cls");
	
	GotoPosition(26, 15);
	ChangeColor(7);
	printf("Press Any Key To Continue");
	system("pause > nul");
	system("cls");
	PlayBGM(1);

	for (index1 = 0; index1 < WINDOWINTERFACE_SIZE_HIGHT; index1++)
	{
		for (index2 = 0; index2 < WINDOWINTERFACE_SIZE_LENGTH; index2++)
		{
			if (index2 == 0 || index2 == GAMEINTERFACE_SIZE_LENGTH - 1 || index2 == WINDOWINTERFACE_SIZE_LENGTH - 1)
			{
				window_intf_t[index2][index1].state = WALL;
				GotoPosition(2 * index2, index1);
				printf("■");
			}
			else if (index1 == WINDOWINTERFACE_SIZE_HIGHT - 1)
			{
				window_intf_t[index2][index1].state = WALL;
				printf("■");
			}
		}
	}

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 19);
	printf("左移: ←");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 17);
	printf("右移: →");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 15);
	printf("旋转: SPACE");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 13);
	printf("暂停: S");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 11);
	printf("退出: ESC");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 9);
	printf("重新开始: R");

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 5);
	printf("分数: %d", g_score);

	GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 3);
	printf("等级: %d", g_game_level);

	for (index_posX = 1; index_posX < GAMEINTERFACE_SIZE_LENGTH; index_posX++)
	{
		for (index_posY = 0; index_posY < GAMEINTERFACE_SIZE_HIGHT; index_posY++)
		{
			if (window_intf_t[index_posX][index_posY].state == BLOCK)
			{
				ChangeColor(window_intf_t[index_posX][index_posY].color);
				GotoPosition(2 * index_posX, index_posY);
				printf("■");
			}
		}
	}

	ChangeColor(g_block_basicstate);
	DrawBlock(g_block_basicstate, rttstate, WINDOWINTERFACE_SIZE_LENGTH - 10, 4);
	ChangeColor(basestate);
	DrawBlock(basestate, rttstate, temp_block_t.position_x, temp_block_t.position_y);
}


//Name:  RestartGame(int basestate, int rttstate, int block_posX, int block_posY)
//Input Type: int * 4
//Return Type: void
//Description: Ask whether is going to restart the game(Y/N)
void RestartGame(int basestate, int rttstate, int block_posX, int block_posY)
{
	int input;

	static int index_posX = 0, index_posY = 0;
	static int index1 = 0, index2 = 0;

	temp_block_t.position_x = block_posX;
	temp_block_t.position_y = block_posY;

	PlayBGM(2);
	system("cls");
	GotoPosition(20, 15);
	ChangeColor(7);
	printf("Are You Sure To Restart The Gmae?(Y/N)");

	while (input = getch())
	{
		if (input == 89 || input == 121)
		{
			system("cls");
			SetInitialBlock();
			GameLoop();
			break;
		}
		if (input == 78 || input == 110)
		{
			system("cls");
			PlayBGM(1);
			ChangeColor(7);
			for (index1 = 0; index1 < WINDOWINTERFACE_SIZE_HIGHT; index1++)
			{
				for (index2 = 0; index2 < WINDOWINTERFACE_SIZE_LENGTH; index2++)
				{
					if (index2 == 0 || index2 == GAMEINTERFACE_SIZE_LENGTH - 1 || index2 == WINDOWINTERFACE_SIZE_LENGTH - 1)
					{
						window_intf_t[index2][index1].state = WALL;
						GotoPosition(2 * index2, index1);
						printf("■");
					}
					else if (index1 == WINDOWINTERFACE_SIZE_HIGHT - 1)
					{
						window_intf_t[index2][index1].state = WALL;
						printf("■");
					}
				}
			}

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 19);
			printf("左移: ←");

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 17);
			printf("右移: →");

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 15);
			printf("旋转: SPACE");

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 13);
			printf("暂停: S");

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 11);
			printf("退出: ESC");

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 9);
			printf("重新开始: R");

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 5);
			printf("分数: %d", g_score);

			GotoPosition(2 * WINDOWINTERFACE_SIZE_LENGTH - 24, WINDOWINTERFACE_SIZE_HIGHT - 3);
			printf("等级: %d", g_game_level);

			for (index_posX = 1; index_posX < GAMEINTERFACE_SIZE_LENGTH; index_posX++)
			{
				for (index_posY = 0; index_posY < GAMEINTERFACE_SIZE_HIGHT; index_posY++)
				{
					if (window_intf_t[index_posX][index_posY].state == BLOCK)
					{
						ChangeColor(window_intf_t[index_posX][index_posY].color);
						GotoPosition(2 * index_posX, index_posY);
						printf("■");
					}
				}
			}

			ChangeColor(g_block_basicstate);
			DrawBlock(g_block_basicstate, rttstate, WINDOWINTERFACE_SIZE_LENGTH - 10, 4);
			ChangeColor(basestate);
			DrawBlock(basestate, rttstate, temp_block_t.position_x, temp_block_t.position_y);

			break;
		}
	}
}

//Name: ESC_QuitGame(void)
//Input Type: void
//Return Type: void
//Description: Quit the game when accept a 'ESC'
void ESC_QuitGame(void)
{
	system("cls");
	PlayBGM(0);
	GotoPosition(24, 15);
	ChangeColor(7);
	printf("You Have Quitted The Qame\n");
	GotoPosition(20, 16);
	printf("Press Any Key To Close This Window");
	system("pause > nul");
	exit(0);
}

//Name: GameFailure(void)
//Input Type: void
//Return Type: void
//Description: React to program when player've failed
void GameFailure(void)
{
	int input;
	
	system("cls");
	PlayBGM(3);
	GotoPosition(26, 15);
	ChangeColor(7);
	printf("You Have Failed The Qame\n");
	GotoPosition(20, 16);
	printf("Do You Want To Restart The Game?(Y/N)");

	while (input = getch())
	{
		if (input == 89 || input == 121)
		{
			system("cls");
			SetInitialBlock();
			GameLoop();
			break;
		}
		if (input == 78 || input == 110)
		{
			system("cls");
			GotoPosition(24, 15);
			ChangeColor(7);
			printf("You Have Quitted The Qame\n");
			GotoPosition(20, 16);
			printf("Press Any Key To Close This Window");
			system("pause > nul");
			exit(0);
		}
	}
}