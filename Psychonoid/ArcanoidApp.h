#pragma once

#include"SDL.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<ctime>
#if defined(WINDOWS) || defined(WIN32) || defined(_WIN32)
#include<windows.h>
#endif
#include "Renderer.h"
#include "PhysicsSimulator.h"
#include "WorldObjectsList.h"
#include "Ship.h"


class CArcanoidApp
{
public:
	SDL_Surface* screen;
	SDL_Event event;

	int lastTime;
	int deltaTime;
	int secondHand;
	int level;
	int lifes;
	int points;
	int fps;
	double shipMaxSpeed;
	double fraction; //for ship
	int width;
	int height;
	double angle;

	int isAccelerating;
	int isTurning_right; 
	int isTurning_left;
	int isShooting;

	int TimeOfLastShoot;
	int TimeOfLastAccelerating;
	int TimeOfLastTurning;
	int TimeOfLastEngineParticle;
	int FractionTimer;

	int particles_color1;
	int particles_color2;
	int asteroid_color;
	int ship_color;
	int bullet_color;
	int engine_color;
	int debug;
	CRenderer *Renderer;
	CPhysicsSimulator *Physics;
	WorldObjectsList *asteroids;
	WorldObjectsList *bullets;
	WorldObjectsList *particles;
	CShip* Ship;

	//defaults
	PolygonVertices* defaultParticle;
	PolygonVertices* defaultAsteroid;
	PolygonVertices* defaultAsteroid2;
	PolygonVertices* defaultAsteroid3;
	int defaultAsteroidColor;
	PolygonVertices* defaultSpaceShip;
	int defaultSpaceShipColor;
	PolygonVertices* defaultBullet;
	Vector defaultShipCenter;

	CArcanoidApp();
	int Random(int min, int max);
	int Initialize(void);
	int Run(void);
	int LoadConfigurationFromFile(char* filename);
	void MainLoop(void);
	void Quit(void);
	int CheckKeysState(void);
	void UpdateTime(void);
	void UpdateObjectsStates(void);
	void CollisionsCheck(void);
	void OutOfBoundsCheck(void);
	void NextLevel();
	void CheckCollisionWithShip(void);
	void CollisionWithShip(CWorldObject *collider);
	void CollisionWithShip(void);
	void CheckCollisionWithBullets(void);
	void CollisionWithBullet(CWorldObject *asteroid, CWorldObject *bullet);
	void CenterShip();
	void Fail();

	void CheckAndUpdateParticles(void);
	void MakeBoomEffect(Vector point, int sizeOfBoom, int color);
	void GenerateAsteroids(int numberOfAsteroids);
	void DrawParticles(void);
	void DrawAsteroids(void);
	void DrawBullets(void);
	void DrawShip(void);
	void HandleEvents(void);

	~CArcanoidApp(void);
};
