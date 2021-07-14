//Name: main.cpp
//Description: The operation of the main program
#include "configuration.h"
#include "headerfileset.h"

int main(void)
{
	CreateWinIntf();
	HideCursor(0);
	LoadDisplay();
	GameLoop();
}