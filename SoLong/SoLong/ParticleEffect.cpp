#include "stdafx.h"
#include "ParticleEffect.h"


int CParticleEffect::Random(int min, int max){
	return (rand()%max)+min; 
}
CParticleEffect::CParticleEffect(int x_, int y_)
{
	x=x_;
	y=y_;
	isAlive=1;
	StartTime = (double)GetTickCount();

//zamienic 5 na random :>?
	Particles=new std::list<CParticle*>;
	for(int i=0; i<10; i++){
		Particles->push_back(new CParticle((double)Random(0,360),(double)Random(5,30)));
	}
}

int CParticleEffect::RenderParticles(int tab[80][50])
{
	double ActualTime = (double)GetTickCount();
	double deltaTime = (ActualTime-StartTime)*.001f;

	if((deltaTime)>(4)) {
		isAlive=0;
	}
	else {

	for(std::list<CParticle*> ::iterator iter=Particles->begin(); iter != Particles->end(); ++iter){
		double dx=0,dy=0;
		double cosalfa = cos((((*iter)->alfa)*3.141592)/180.0f);
		double sinalfa = sin((((*iter)->alfa)*3.141592)/180.0f);
	
		dx=cosalfa*(*iter)->velocity*(double)deltaTime;
		dy=sinalfa*(*iter)->velocity*(double)deltaTime+(5.0f*(double)deltaTime*(double)deltaTime);
		if ((x+(int)dx)<LEFT_BORDER+1 || (x+(int)dx)>RIGHT_BORDER-1 || y+(int)dy>DOWN_BORDER-1 || y+(int)dy<UP_BORDER+1) continue;
		else {
			switch(Random(1,4)) {		
			case 1:
				if (tab[x+(int)dx][y+(int)dy]==WALL){
					tab[x+(int)dx][y+(int)dy]=PARTICLE1_ON_WALL;}
				else tab[x+(int)dx][y+(int)dy]=PARTICLE1;
				break;
			case 2:
				if (tab[x+(int)dx][y+(int)dy]==WALL){
					tab[x+(int)dx][y+(int)dy]=PARTICLE2_ON_WALL;}
				else tab[x+(int)dx][y+(int)dy]=PARTICLE2;
				break;
			case 3:
				if (tab[x+(int)dx][y+(int)dy]==WALL){
					tab[x+(int)dx][y+(int)dy]=PARTICLE3_ON_WALL;}
				else tab[x+(int)dx][y+(int)dy]=PARTICLE3;
				break;
			case 4:
				if (tab[x+(int)dx][y+(int)dy]==WALL){
					tab[x+(int)dx][y+(int)dy]=PARTICLE4_ON_WALL;}
				else tab[x+(int)dx][y+(int)dy]=PARTICLE4;
				break;

			}
		}
	}

	}
	return 0;
}
CParticleEffect::~CParticleEffect(void)
{
	for(std::list<CParticle*> ::iterator iter=Particles->begin(); iter != Particles->end();) {
		CParticle* temp = *iter;
		iter = Particles->erase(iter);
		delete temp;
		}
	delete Particles;
}
