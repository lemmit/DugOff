#include "StdAfx.h"
#include "Renderer.h"

CRenderer::CRenderer(void)
{
	RenderingMode=false;
	//Zapelniamy tablice zerami na poczatek
	for(int y=0; y<50; y++){
		for(int x=0; x<80; x++)
			{
			if(!((x==79) && (y==49)))
			{
			LastRendered[x][y]=0;
			}
			else continue;
		}
}
	setTextAttributes(0x0F);
	//printf("Renderer stworzony\n");
}
int CRenderer::ChangeTrapRenderMask(void){
	//Czyscimy tablice, aby wymusic wyrenderowanie calosci jeszcze raz
	for(int y=0; y<50; y++){
		for(int x=0; x<80; x++)
			{
			if(!((x==79) && (y==49)))
			{
			LastRendered[x][y]=0;
			}
			else continue;
		}
	}

	if (RenderingMode) RenderingMode=false;
	else RenderingMode=true;
return 0;
}
int CRenderer::RenderTrapsNearSnake(int tab[80][50], int x0, int y0){
	int w=5;
	int h=5;
	x0-=2;
	y0-=2;
	//Czyscimy w okolo snake'a 
for(int y=y0-1;y<(y0+h) ;y++){
	for(int x=x0-1;x<(x0+w);x++) {
		switch(tab[x][y])
				{
					case TRAP:
						gotoXY(x,y);
						if(RenderingMode) write('.'); 
						else write(' ');  
						break;
					default:
						break;
				}
		}
	}
//Rysujemy pulapki dookola snake'a 
for(int y=y0+1;y<(y0+h-1) ;y++){
	for(int x=x0+1;x<(x0+w-1);x++) {
		switch(tab[x][y])
				{
					case TRAP:
						gotoXY(x,y);
						write('P'); 
						break;
					default:
						break;
				}
		}
	}
return 0;
}

int CRenderer::Render(int tab[80][50]) {
	setTextAttributes(0x0F);
	//Renderujemy, ale tylko to co sie zmienilo w stosunku do ostatniego renderu
	int SnakeHead_x=0, SnakeHead_y=0;
	for(int y=0; y<50; y++){
		for(int x=0; x<80; x++)
		{
			if(!((x==79) && (y==49)))
			{
				if(LastRendered[x][y]!=tab[x][y]){
				switch(tab[x][y])
				{
					case EMPTY:
						gotoXY(x,y);
						write(' ');
						break;
						
					case WALL:
						gotoXY(x,y);
						write(219);
						break;

					case SNAKEHEAD:
						setTextAttributes(FOREGROUND_GREEN);
						gotoXY(x,y);
						SnakeHead_x=x;
						SnakeHead_y=y;
						write('H');
						break;				

					case SNAKEBODY:
						setTextAttributes(FOREGROUND_GREEN);
						gotoXY(x,y);
						write('X');
						break;
	
					case TREASURE:
						setTextAttributes(FOREGROUND_RED);
						gotoXY(x,y);
						write('S');
						break;
						
					case TRAP:
						gotoXY(x,y);
						if (RenderingMode)
						write('.'); 
						else write(' ');
						break;

					case PARTICLE1:
						setTextAttributes(FOREGROUND_RED);
						gotoXY(x,y);
						write('^');
						break;
						
					case PARTICLE2:
						setTextAttributes(FOREGROUND_GREEN);
						gotoXY(x,y);
						write('\"');
						break;

					case PARTICLE3:
						setTextAttributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						gotoXY(x,y);
						write('+');
						break;
						
					case PARTICLE4:
						setTextAttributes(FOREGROUND_BLUE);
						gotoXY(x,y);
						write('#');
						break;

					case PARTICLE1_ON_WALL:
						setTextAttributes(FOREGROUND_RED | 0xF0);
						gotoXY(x,y);
						write('^');
						break;
						
					case PARTICLE2_ON_WALL:
						setTextAttributes(FOREGROUND_GREEN | 0xF0);
						gotoXY(x,y);
						write('\"');
						break;

					case PARTICLE3_ON_WALL:
						setTextAttributes(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | 0xF0);
						gotoXY(x,y);
						write('+');
						break;
						
					case PARTICLE4_ON_WALL:
						setTextAttributes(FOREGROUND_BLUE  | 0xF0);
						gotoXY(x,y);
						write('#');
						break;


					default:
						gotoXY(x,y);
						write((char)tab[x][y]);
						break;
				}
				setTextAttributes(0x0F);
				}
			//	gotoXY(79,49);
			}
		}
	}
	RenderTrapsNearSnake(tab,SnakeHead_x,SnakeHead_y);
	memcpy(LastRendered,tab,50*80*sizeof(int));
	return 0;
}

CRenderer::~CRenderer(void)
{
}
