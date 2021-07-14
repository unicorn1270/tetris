//Name: configuration.h
//Description: Declare the constants, variables and functions used by the program
//Declare external links

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

/*Interface Setting*/

#define WINDOWINTERFACE_SIZE_LENGTH 40
#define WINDOWINTERFACE_SIZE_HIGHT 30
#define GAMEINTERFACE_SIZE_LENGTH 25
#define GAMEINTERFACE_SIZE_HIGHT 30
#define GAME_INTF_X 25//game interface position X
#define GAME_INTF_Y 30//game interface position Y

/*Function Key*/

#define DIR_LEFT 75
#define DIR_RIGHT 77
#define DIR_DOWN 80
#define SPACE 32
#define ESC 27

/*Game Parameters Setting*/

#define BLANK 0
#define BLOCK 1
#define WALL 2
#define BASEBLOCKS 7//seven basic blocks
#define BLOCKS_SIZE 4//size of block: 4*4
#define RTTSTATES 4//four rotating states

/*Set Gobal Variables*/

extern int g_score;
extern int g_game_level;
extern int g_block_basicstate;
extern int g_fallspeed;

/*Set Structure*/
//set window interface parameters
struct WINDOW_INTF
{
	int state;
	int color;
};

//set blocks parameters
struct BLOCKS
{
	int formation[BLOCKS_SIZE][BLOCKS_SIZE];
};

//store temp block's position(when "stop")
struct TEMP_BLOCK
{
	int position_x;
	int position_y;
};

/*Set Functions*/

bool ClearBlocks(void);
void CreateWinIntf(void);
void ESC_QuitGame(void);
void GameFailure(void);
void GameLoop(void);
void GameLevelCnt(void);
void InitBlocks(void);
void InitWindowIntf(void);
void LoadDisplay(void);
void PlayBlockHitMusic(void);
void StartGame(void);
void SetInitialBlock(void);

void ChangeColor(int);
void DrawBlank(int, int, int, int);
void DrawBlock(int, int, int, int);
void GotoPosition(int, int);
void GetChoice(int*, int*, int*, int*);
void HideCursor(int);
bool IsBlocksHit(int, int, int, int);
void PlayBGM(int);
void PauseGame(int, int, int, int);
void RestartGame(int, int , int, int);

#endif