#pragma once

#include "Vector.h"
#include "Polygon.h"
#include <cmath>

class CWorldObject{
public:
	
	PolygonVertices* polygon;//wierzcholki wielokata
	double angvelocity;
	double radius;
	int isBullet;
	int isCollidable;
	int isDying;
	int type;
	int liveTime;
	Vector velocity;
	Vector center;

	CWorldObject();
	CWorldObject(PolygonVertices* _polygon, Vector _velocity, double _angvelocity, int _isBullet, int _isCollidable);
	CWorldObject(PolygonVertices* _polygon, double velocity_x, double velocity_y, double _angvelocity, int _isBullet, int _isCollidable);

	Vector CalculateCenter();
	double CalculateRadius();
	
	void Die();
	void SetObject(PolygonVertices* _polygon, Vector _velocity, double _angvelocity, int _isBullet, int _isCollidable);
	void SetObject(PolygonVertices* _polygon, double velocity_x, double velocity_y, double _angvelocity, int _isBullet, int _isCollidable);

	//more sets!!!!

	virtual ~CWorldObject(void);

};
