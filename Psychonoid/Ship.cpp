#include "Ship.h"

CShip::CShip(PolygonVertices* _polygon, double velocity_x, double velocity_y, 
								  double _angvelocity, int _isBullet, int _isCollidable)
	: CWorldObject(_polygon,velocity_x,velocity_y,_angvelocity,_isBullet,_isCollidable)
{
actual_angle=0;
}

CShip::~CShip(void)
{
}
