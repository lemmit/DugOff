// SoLong.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int key;
	int WannaPlay=1;
	do{
		CGame* SnakeGame = new CGame;
		WannaPlay=SnakeGame->Start();
		
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		delete SnakeGame;
		gotoXY(20,31);
		printf("Want to play again?...ESC/ENTER");
		while(1){
			if (kbhit()){
				key = getVKey();
				if (key==VK_RETURN) {
					WannaPlay=1; break;
				}
				else if (key==VK_ESCAPE){
					WannaPlay=0; break;
				}
			}
		}
	}while(WannaPlay);
	gotoXY(20,31);
	printf("                                 ");
	gotoXY(20,31);
	printf("OK. Bye Bye.");
	gotoXY(0,49);
	key=getVKey();
	return 0;
}


