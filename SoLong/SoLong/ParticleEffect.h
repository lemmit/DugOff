#pragma once
#include "stdafx.h"

class CParticleEffect
{
public:
	CParticleEffect(int,int);
	int RenderParticles(int tab[80][50]);
	int isAlive;
	~CParticleEffect(void);
private:
	int x,y;
	double StartTime;
	std::list<CParticle*> *Particles;
	int Random(int min, int max);
	
};
