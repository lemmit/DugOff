// Prosta biblioteka konsolowa - win32
// Autor: Jacek Dabrowski, jacek.dabrowski@eti.pg.gda.pl
// Wyrazam zgode na wykorzystanie i modyfikacje tej biblioteki przez studentow pierwszego 
// roku Informatyki Politechniki Gdanskiej w ramach przedmiotu Praktyka Programowania

#include "stdafx.h"

HANDLE hStdout, hStdin; 

// the key record is kept between calls to getKey/getVKey to handle
// multiple same characters in a single event
KEY_EVENT_RECORD ker;

// Initialize the console and set it to full screen mode
BOOL initConsole()
{
	hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE || 
        hStdout == INVALID_HANDLE_VALUE) 
    {
        MessageBox(NULL, TEXT("GetStdHandle"),
            TEXT("Console Error"), MB_OK);
        return false;
    }

	ker.wRepeatCount = 0;

	SMALL_RECT windowSize = {0, 0, 79, 49};
    
    // Change the console window size:
    SetConsoleWindowInfo(hStdout, TRUE, &windowSize);

	COORD c = { 80, 50};

    //Change the internal buffer size:
    SetConsoleScreenBufferSize(hStdout, c);

	
	SetConsoleDisplayMode(hStdout,CONSOLE_FULLSCREEN_MODE, &c);

	//Hide Cursor in ConsoleWindow
    CONSOLE_CURSOR_INFO cursor = {1, 0}; 
    SetConsoleCursorInfo(hStdout, &cursor); 

	return true;
}

// return the current column
int whereX()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hStdout, &info);
	return info.dwCursorPosition.X;
}

// return the current row
int whereY()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hStdout, &info);
	return info.dwCursorPosition.Y;
}

// return the width of the screen
int sizeX()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hStdout, &info);
	return info.dwSize.X;
}

// return the height of the screen
int sizeY()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hStdout, &info);
	return info.dwSize.Y;
}

// move the cursor to this position
void gotoXY(int x, int y)
{
	COORD c;
	c.X=x;
	c.Y=y;
	SetConsoleCursorPosition(hStdout,c);

}

// set the text attributes to the specified value 
// (check documentation for SetConsoleTextAttribute for details)
void setTextAttributes(int attr)
{
	SetConsoleTextAttribute(hStdout, (WORD) attr);
}

// return the current text attributes
int getTextAttributes()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hStdout, &info);
	return info.wAttributes;
}

// write a single character to screen
void write(char c)
{
	DWORD charsWritten;
	WriteConsole(hStdout, &c, 1, &charsWritten, NULL);
}

// write a null-terminated string to screen
void write(char *str)
{
	DWORD charsWritten;
	WriteConsole(hStdout, str, strlen(str), &charsWritten, NULL);
}


// fill the screen using current attributes
void fillScreen()
{
	DWORD charsWritten;
	COORD c;					//upper left corner
	c.X=0;
	c.Y=0;

	int size = sizeX()*sizeY(); //number of characters in the screen

	//fill the screen with spaces
	FillConsoleOutputCharacter(hStdout, ' ', size, c, &charsWritten);

	//set the attribute to current attribute 
	int attr = getTextAttributes();
	FillConsoleOutputAttribute(hStdout, attr, size, c, &charsWritten);

	//put the cursor in the upper left corner
	gotoXY(0,0);
}



// clear the screen using default attributes (reset to defaults as well)
void clearScreen()
{
	setTextAttributes(FOREGROUND_BLUE |FOREGROUND_GREEN |FOREGROUND_RED );

	fillScreen();
}


// return the ASCII code for the key pressed
char getKey()
{
	// if there are some characters left in the last keyboard event
	if (ker.wRepeatCount > 0)
	{
		ker.wRepeatCount --;
		return ker.uChar.AsciiChar;
	}

	INPUT_RECORD inputInfo;
	while(1)
	{
		DWORD count;
		BOOL result = ReadConsoleInput(hStdin, &inputInfo, 1, &count);

		if (count==1)
		{
			if (inputInfo.EventType == KEY_EVENT)
			{
				ker = inputInfo.Event.KeyEvent;
				if (ker.bKeyDown)
				{
					ker.wRepeatCount --;
					return ker.uChar.AsciiChar;
				}
			}
		}
		else
			Sleep(25);
	}
}

bool kbhit()
{
  DWORD en, enr; // liczba zdarzeñ oraz liczba odczytanych zdarzeñ

  GetNumberOfConsoleInputEvents(hStdin, &en);
  INPUT_RECORD * ir = new INPUT_RECORD[en];  // przydzielamy pamiêæ
  PeekConsoleInput(hStdin, ir, en, &enr);     // odczytujemy zdarzenia

  for(int i = 0; i < enr; i++)               // przegl¹damy zdarzenia
  {
    if(ir[i].EventType & KEY_EVENT && 
       ir[i].Event.KeyEvent.bKeyDown)        // jeœli zdarzenie klawisza!
    {                                        // ale nie kontrolnego!
      if(ir[i].Event.KeyEvent.wVirtualKeyCode != VK_CONTROL &&
         ir[i].Event.KeyEvent.wVirtualKeyCode != VK_MENU &&
         ir[i].Event.KeyEvent.wVirtualKeyCode != VK_SHIFT)
      {
        delete ir; return true;              // klawisz w buforze konsoli
      }
    }
  }
  delete ir; return false;                   // bufor konsoli jest pusty
}



// return the Keyboard code for the key pressed (Virtual-Key Codes)
int getVKey()
{
	if (ker.wRepeatCount > 0)
	{
		ker.wRepeatCount --;
		return ker.wVirtualKeyCode;
	}

	// if there are some characters left in the last keyboard event
	INPUT_RECORD inputInfo;
	while(1)
	{
		DWORD count;
		BOOL result = ReadConsoleInput(hStdin, &inputInfo, 1, &count);
		if (count==1)
		{
			if (inputInfo.EventType == KEY_EVENT)
			{
				ker = inputInfo.Event.KeyEvent;
				if (ker.bKeyDown)
				{
					ker.wRepeatCount --;
					return ker.wVirtualKeyCode;
				}
			}
		}
		
		else break;//Sleep(25);
	}
}