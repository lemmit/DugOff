#pragma once
#include "worldobject.h"

class CShip :
	public CWorldObject
{
public:
	double actual_angle;
	CShip(PolygonVertices* _polygon, double velocity_x, double velocity_y, 
								  double _angvelocity, int _isBullet, int _isCollidable);
	~CShip(void);
};
