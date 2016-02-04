#pragma once
#include "worldobject.h"

class CAsteroid :
	public CWorldObject
{
public:
	int Draw();
	CAsteroid(SDL_Surface* _screen);
	~CAsteroid(void);

};
