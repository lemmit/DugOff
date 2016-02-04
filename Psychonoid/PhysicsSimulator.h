#pragma once
#include "Vector.h"
#include "Polygon.h"
#include "WorldObject.h"

class CPhysicsSimulator
{
public:
	int width;
	int height;
	CPhysicsSimulator(int _width,int _height);
	int UpdateState(CWorldObject* worldobject, int deltaTime);
	
	int CheckCollision(CWorldObject* first, CWorldObject* second);

	int CheckVectorCollision(Vector start_1, Vector end_1, Vector start_2, Vector end_2);

	int CheckPolygonsCollisions(PolygonVertices* polygon1, PolygonVertices* polygon2);

	int CheckCircleCollisions(Vector center1, double radius1, Vector center2, double radius2);

	inline double DistanceBetweenPoints(Vector* v1, Vector* v2);

	int IsStickingOut(PolygonVertices* polygon);

	void TranslatePoint(Vector* point, Vector vec2);
	void TranslatePolygon(PolygonVertices* polygon1, Vector vec2);
	void RotatePoint(Vector* point, Vector point_zero, double angle);
	void RotatePolygon(PolygonVertices* polygon, Vector center, double angle);
	void RotateWorldObject(CWorldObject* worldobject, double angle);
	void TranslateWorldObject(CWorldObject* worldobject, Vector vec2);
	
	//out of bound collision chyba najlepiej te¿.

	//lepiej w APPie?
	//int ShipAndAsteroidsCollisions(Asteroids *asteroids, CShip *ship);
	//int AsteroidsAndBulletsCollisions(Asteroids* asteroids, Bullets* bullets);

	~CPhysicsSimulator(void);
};
