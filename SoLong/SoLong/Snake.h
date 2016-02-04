#pragma once

class CSnake
{
public:
	CSnakeElement* SnakesHead;
	CSnake(void);
	int RenderSnake(int tab[80][50]);
	int MoveUp(int tab[80][50]);
	int MoveDown(int tab[80][50]);
	int MoveRight(int tab[80][50]);
	int MoveLeft(int tab[80][50]);
	int Move(int tab[80][50]);
	void Start(void);
	~CSnake(void);
private:
	std::list<CSnakeElement*> *SnakeElements;
	Duration HeadDuration;
	

};
