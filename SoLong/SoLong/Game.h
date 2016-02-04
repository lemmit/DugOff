#pragma once

class CGame
{
public:
	CGame(void);
	int Start(void);
	~CGame(void);
private:
	CRenderer *Renderer;
	CSnake *Snake;
	std::list<CParticleEffect*> *ParticleEffects;
	int BaseTab[80][50];
	int TabToRender[80][50];
	int key;
	int points;
	bool SnakeMode;
	int TimeFromLastClick;
	int GameEnd(void);
	int Random(int,int);
	int AddTreasure(void);
	int AddTrap(void);
	int AddWall(void);
	int AddParticleEffect(int,int);
	int AddCredits(void);
	int CheckCollision(int,int,int,int);
	int ClearScreen(void);
	void ChangeSnakeMode(void);
	void CollectTreasure(void);
	void CheckParticleEffects(void);
};
