#include "StdAfx.h"
#include "Game.h"

CGame::CGame(void)
{

//Zapelniamy tablice zerami na poczatek
for(int y=0; y<50; y++){
		for(int x=0; x<80; x++)
			{
			if(!((x==79) && (y==49)))
			{
			BaseTab[x][y]=0;
			}
			else continue;
		}
}
//Wypelniamy zerami druga tablice
for(int y=0; y<50; y++){
		for(int x=0; x<80; x++)
			{
			if(!((x==79) && (y==49)))
			{
			TabToRender[x][y]=0;
			}
			else continue;
		}
}
//Zerowanie punktów
points=0;
//printf("Utworzono obiekt Gry!\n");
Renderer = new CRenderer;
Snake = new CSnake;
ParticleEffects = new std::list<CParticleEffect*>;
srand((unsigned)time(0));
TimeFromLastClick=0;
SnakeMode=0;
}
void CGame::ChangeSnakeMode(void){
	if (SnakeMode) SnakeMode=false;
	else SnakeMode=true;
}

int CGame::AddCredits(void){
	gotoXY(3,0);
	printf("Michal Kupiec 131533                           Punkty: %d", points);
	return 0;
}
int CGame::Random(int min, int max){
	return (rand()%max)+min; 
}
int CGame::CheckCollision(int x, int y, int w, int h){
	//Sprawdza czy na tablicy gry na prostok¹cie oznaczonym przez x,y i width/height coœ siê znajduje
	int ok=1;
	int temp;
	for(int i=x;i<x+w;i++){
		for(int j=y;j<y+h;j++){
			temp=TabToRender[i][j];
			if(temp==EMPTY || temp==PARTICLE1 || temp==PARTICLE2 || temp==PARTICLE3 || temp==PARTICLE4) continue;
			else ok=0;
		}
	}
	return ok;
}
int CGame::AddTreasure(void){
	//Dodaje randomowany Skarb
	int x=0,y=0;
	int temp=0;
		do{
			x=Random(LEFT_BORDER+1,RIGHT_BORDER-1);
			y=Random(UP_BORDER+1,DOWN_BORDER-3);
			temp=TabToRender[x][y];
		}while(!(temp==EMPTY || temp==PARTICLE1 || temp==PARTICLE2 || temp==PARTICLE3 || temp==PARTICLE4));
	BaseTab[x][y]=TREASURE;

	return 0;
}
int CGame::AddTrap(void){
	//Dodaje randomow¹ pu³apkê
	int x=0,y=0;
	int temp=0;
		do{
			x=Random(LEFT_BORDER+1,RIGHT_BORDER-1);
			y=Random(UP_BORDER+1,DOWN_BORDER-3);
			temp=TabToRender[x][y];
		}while(!(temp==EMPTY || temp==PARTICLE1 || temp==PARTICLE2 || temp==PARTICLE3 || temp==PARTICLE4));
	BaseTab[x][y]=TRAP;
	return 0;
}
int CGame::AddWall(void){
	//Dodaje œcianê
	int x=0,y=0,h=0,w=0;
	int VerticalOrHorizontal=Random(1,2);
	if (VerticalOrHorizontal==1){//Horizontal Wall
		do{
			h=1;
			w=Random(2,10);
			x=Random(LEFT_BORDER,RIGHT_BORDER-w);
			y=Random(UP_BORDER,DOWN_BORDER-1);
		}while(!CheckCollision(x,y,w,h));
		//Adding Wall
			for(int i=x;i<x+w;i++) 
				for(int j=y;j<y+h;j++) BaseTab[i][j]=WALL;
		} 
	else {//Vertical Wall
		do{
			w=1;
			h=Random(2,10);
			x=Random(LEFT_BORDER,RIGHT_BORDER-1);
			y=Random(UP_BORDER,DOWN_BORDER-h);
		}while(!CheckCollision(x,y,w,h));
		//Adding Wall
				for(int i=x;i<x+w;i++) 
					for(int j=y;j<y+h;j++) BaseTab[i][j]=WALL;
		} 
	return 0;
}

int CGame::AddParticleEffect(int x,int y){
	//Dodaje efekt cz¹steczkowy na x,y
	ParticleEffects->push_back(new CParticleEffect(x,y));
	return 0;
}
void CGame::CheckParticleEffects(void){
	for(std::list<CParticleEffect*>::iterator iter=ParticleEffects->begin(); iter != ParticleEffects->end();)
	{
		if((*iter)->isAlive!=1) 
		{
			CParticleEffect* temp = *iter;
			iter = ParticleEffects->erase(iter);
			//gotoXY(50,49);
			//printf("Effect Destroyed");
			delete temp;
		}
		else {
			(*iter)->RenderParticles(TabToRender);
			//gotoXY(50,49);
			//printf("Effect Rendered");
			iter++;

		}
	}
}
int CGame::ClearScreen(void){
	//ClearScreen raz na sekunde
	return 0;
}
void CGame::CollectTreasure(void){
		BaseTab[Snake->SnakesHead->x][Snake->SnakesHead->y]=EMPTY;
		AddParticleEffect(Snake->SnakesHead->x,Snake->SnakesHead->y);
		CheckParticleEffects();
		AddTreasure();
		points+=1;
}

int CGame::Start(void)
{
	int a=30;
	//printf("Start!\n");
	initConsole();

//Rysujemy ramke glowna
for(int x=LEFT_BORDER; x<=RIGHT_BORDER; x++){
			BaseTab[x][UP_BORDER]=WALL;
			BaseTab[x][DOWN_BORDER]=WALL;
			}

for(int y=UP_BORDER; y<=DOWN_BORDER; y++){
			BaseTab[LEFT_BORDER][y]=WALL;
			BaseTab[RIGHT_BORDER][y]=WALL;
			}

//memcpy(dest,source,length);
memcpy(TabToRender,BaseTab,50*80*sizeof(int));
//Tworzymy pierwszy element snake'a / gracza.
Snake->Start();
	clearScreen();
	memcpy(TabToRender,BaseTab,50*80*sizeof(int));
	Snake->RenderSnake(TabToRender);
	AddCredits();
	AddTreasure();
	Renderer->Render(TabToRender);
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
	DWORD count=0;
while(1){
//Podstawowa tablice kopiujemy na nowego taba
	//Check keyboard events	
	key=0;
	if (kbhit()) {
		key = getVKey();
		TimeFromLastClick=GetTickCount();
	}
	else if (((double(GetTickCount()) - (double)TimeFromLastClick)*0.001f>0.25f) && SnakeMode==1){
		switch(Snake->Move(TabToRender)){
					case TREASURE:
						CollectTreasure();
						break;
					case TRAP:
						GameEnd();
						return 0;
						break;
					case SNAKEBODY:
						GameEnd();
						return 0;
						break;
		}
			TimeFromLastClick=GetTickCount();
	}
	FlushConsoleInputBuffer(hStdin);
		switch (key)
		{
			case 0: break;
			case VK_UP :
				switch(Snake->MoveUp(TabToRender)){
					case TREASURE:
						CollectTreasure();
						break;
					case TRAP:
						GameEnd();
						return 0;
						break;
					case SNAKEBODY:
						GameEnd();
						return 0;
						break;
				}
				break;
			case VK_DOWN:
				switch(Snake->MoveDown(TabToRender)){
					case TREASURE:
						CollectTreasure();
						break;
					case TRAP:
						GameEnd();
						return 0;
						break;
					case SNAKEBODY:
						GameEnd();
						return 0;
						break;
				}
				break;
			case VK_RIGHT:
				switch(Snake->MoveRight(TabToRender)){
					case TREASURE:
						CollectTreasure();
						break;
					case TRAP:
						GameEnd();
						return 0;
						break;
					case SNAKEBODY:
						GameEnd();
						return 0;
						break;
				}
				break;
			case VK_LEFT:

				switch(Snake->MoveLeft(TabToRender)){
					case TREASURE:
						CollectTreasure();
						break;
					case TRAP:
						GameEnd();
						return 0;
						break;
					case SNAKEBODY:
						GameEnd();
						return 0;
						break;
						}
				break;
			case VK_KEY_T:
				AddTrap();
				break;
			case VK_KEY_N:
				//AddTreasure(); blocked
				ChangeSnakeMode();
				break;
			case VK_KEY_S:
				AddWall();
				break;
			case VK_KEY_P:
				Renderer->ChangeTrapRenderMask();
				break;
			case VK_ESCAPE:
				return 0;
				break;
			default:
				break;
		}

	memcpy(TabToRender,BaseTab,50*80*sizeof(int));
	
	Snake->RenderSnake(TabToRender);

	AddCredits();
	CheckParticleEffects();
	Renderer->Render(TabToRender);
	Sleep(5);
	}
return 0;
}

int CGame::GameEnd(void){
	memcpy(TabToRender,BaseTab,50*80*sizeof(int));
	Snake->RenderSnake(TabToRender);
	AddCredits();
	Renderer->Render(TabToRender);
	gotoXY(20,30);
	printf("YOU HAVE LOST THE GAME. YOU SCORE: %d\n", points);
ClearScreen();
return 0;
}

CGame::~CGame(void)
{
	delete Renderer;
	delete Snake;
	for(std::list<CParticleEffect*>::iterator iter=ParticleEffects->begin(); iter != ParticleEffects->end();){
			CParticleEffect* temp = *iter;
			iter = ParticleEffects->erase(iter);
			delete temp;
	}
	delete ParticleEffects;
}
