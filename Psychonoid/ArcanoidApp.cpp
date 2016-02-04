
#include "ArcanoidApp.h"

CArcanoidApp::CArcanoidApp()
{
	angle = 0;
	points = 0;
	fps = 0;
	secondHand = 0;
	level=0;
	debug=1;
	isAccelerating=0; 
	isTurning_right=0; 
	isTurning_left=0;
	isShooting=0;

	TimeOfLastShoot=0;
	TimeOfLastAccelerating=0;
	TimeOfLastTurning=0;
	FractionTimer=0;
	TimeOfLastEngineParticle=0;

	fraction=0.99;
	shipMaxSpeed=0.8;

	particles_color1=0xff0000;
	particles_color2=0xffff00;
	asteroid_color=0xffff00;
	ship_color=0xff0000;
	bullet_color=0xffffff;
	engine_color=0x0303ff;


	asteroids = new WorldObjectsList;
	bullets = new WorldObjectsList;
	particles = new WorldObjectsList;

	defaultAsteroid = new PolygonVertices;
	defaultAsteroid2 = new PolygonVertices;
	defaultAsteroid3 = new PolygonVertices;

	defaultSpaceShip = new PolygonVertices;
	defaultBullet = new PolygonVertices;
	defaultParticle = new PolygonVertices;

	//domyslny statek
	defaultSpaceShip->push_back(new Vector(285,198));
	defaultSpaceShip->push_back(new Vector(290,211));
	defaultSpaceShip->push_back(new Vector(296,211));
	defaultSpaceShip->push_back(new Vector(299,206));
	defaultSpaceShip->push_back(new Vector(299,217));
	defaultSpaceShip->push_back(new Vector(290,214));
	defaultSpaceShip->push_back(new Vector(292,219));
	defaultSpaceShip->push_back(new Vector(279,219));
	defaultSpaceShip->push_back(new Vector(282,214));
	defaultSpaceShip->push_back(new Vector(273,217));
	defaultSpaceShip->push_back(new Vector(273,206));
	defaultSpaceShip->push_back(new Vector(276,211)); 
	defaultSpaceShip->push_back(new Vector(282,211));
	defaultSpaceShip->push_back(NULL);

	//srodek statku
	defaultShipCenter.x=285;
	defaultShipCenter.y=214;

	//asteroid domyslny
	defaultAsteroid->push_back(new Vector(475,91));
	defaultAsteroid->push_back(new Vector(510,102));
	defaultAsteroid->push_back(new Vector(502,133));
	defaultAsteroid->push_back(new Vector(515,164));
	defaultAsteroid->push_back(new Vector(464,164));
	defaultAsteroid->push_back(new Vector(436,125));
	defaultAsteroid->push_back(NULL);

	//asteroid2 domyslny
	defaultAsteroid2->push_back(new Vector(475,91));
	defaultAsteroid2->push_back(new Vector(510,102));
	defaultAsteroid2->push_back(new Vector(502,133));
	defaultAsteroid2->push_back(new Vector(436,125));
	defaultAsteroid2->push_back(NULL);

	//asteroid3 domyslny
	defaultAsteroid3->push_back(new Vector(475,91));
	defaultAsteroid3->push_back(new Vector(510,102));
	defaultAsteroid3->push_back(new Vector(502,133));
	defaultAsteroid3->push_back(new Vector(480,115));
	defaultAsteroid3->push_back(NULL);



	//domyslny naboj
	defaultBullet->push_back(new Vector(0,0));
	defaultBullet->push_back(new Vector(2,0));
	defaultBullet->push_back(new Vector(2,6));
	defaultBullet->push_back(new Vector(0,6));
	defaultBullet->push_back(NULL);
	
	//domyslna czasteczka
	defaultParticle->push_back(new Vector(303,94));
	defaultParticle->push_back(new Vector(302,100));
	defaultParticle->push_back(new Vector(297,96));
	defaultParticle->push_back(NULL);

	lifes = 3;
}

int CArcanoidApp::Initialize(void){

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		printf("error: %s\n", SDL_GetError());
		return 1;
	}
	if (TTF_Init() == -1) {
		printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
		return 1;
	}
	LoadConfigurationFromFile("config.pst");
	
	width=800;
	height=600;
	Renderer=new CRenderer(width,height);

	return 0;
}

void CArcanoidApp::UpdateTime(void){
		int tempTime=0;
		tempTime = SDL_GetTicks();
		deltaTime=tempTime-lastTime;
		lastTime = tempTime;
		if(secondHand<=1000) secondHand+=deltaTime;
}

void CArcanoidApp::UpdateObjectsStates(void){
		Vector tempvector(0,0);

//tarcie - statek zwalnia
	if(SDL_GetTicks()-FractionTimer>20){
			Ship->velocity.x*=fraction; 
			Ship->velocity.y*=fraction; 
			FractionTimer=SDL_GetTicks();
		}
	
		//lecimy po listach i ze wzgledu na v/angv ruszamy itp.
//czyscimy listy z umarlych obiektów
		CWorldObject* temp;
		for(WorldObjectsList::iterator iter=asteroids->begin(); iter != asteroids->end();) {
			if((*iter)->isDying == 1){
				temp=(*iter);
				iter = asteroids->erase(iter);
				delete temp;
			}
			else ++iter;
		}

		for(WorldObjectsList::iterator iter=bullets->begin(); iter != bullets->end();) {
			if((*iter)->isDying == 1){
				temp=(*iter);
				iter = bullets->erase(iter);
				delete temp;
			}
			else ++iter;
		}

		//polozenie statku
		tempvector.x=Ship->velocity.x*deltaTime;
		tempvector.y=Ship->velocity.y*deltaTime;
		Physics->TranslateWorldObject(Ship,tempvector);
		Physics->RotatePolygon(Ship->polygon,Ship->center,Ship->angvelocity*deltaTime);
		Ship->actual_angle+=Ship->angvelocity*deltaTime;

		//polozenie asteroid
		for(WorldObjectsList::iterator iter=asteroids->begin(); iter != asteroids->end(); iter++) {
			//polozenie
			tempvector.x=(*iter)->velocity.x*deltaTime;
			tempvector.y=(*iter)->velocity.y*deltaTime;
			Physics->TranslateWorldObject((*iter),tempvector);
			Physics->RotatePolygon((*iter)->polygon,(*iter)->CalculateCenter(), ((*iter)->angvelocity*deltaTime));

		}

		//polozenie naboi
		for(WorldObjectsList::iterator iter=bullets->begin(); iter != bullets->end(); ) {
			//polozenie
			if((*iter)->liveTime<800){
				tempvector.x=(*iter)->velocity.x*deltaTime;
				tempvector.y=(*iter)->velocity.y*deltaTime;
				(*iter)->liveTime+=deltaTime;
				(*iter)->CalculateCenter();
				Physics->TranslateWorldObject((*iter),tempvector);
				iter++;
			}
			else {
				temp=(*iter);
				iter=bullets->erase(iter);
				delete temp;
			}
		}


}

void CArcanoidApp::CollisionsCheck(void){
 //kolizje ze statkiem, i bulletów z asteroidami
	CheckCollisionWithShip();
	CheckCollisionWithBullets();
}

void CArcanoidApp::CollisionWithBullet(CWorldObject *asteroid, CWorldObject *bullet){
		CWorldObject *tempObject;
		Vector tempVector;
	Renderer->Circle(bullet->CalculateCenter(),5., 0xffffff);
/*	Renderer->Circle(asteroid->CalculateCenter(),10., 0xffffff);
	Renderer->Circle(asteroid->CalculateCenter(),12., 0xffffff);
	Renderer->Circle(asteroid->CalculateCenter(),13., 0xffffff);
	Renderer->Circle(asteroid->CalculateCenter(),15., 0xffffff);*/
	MakeBoomEffect(asteroid->center, 40, asteroid_color);
	bullet->isDying=1;
	asteroid->isDying=1;
	asteroid->isCollidable=0;
	switch(asteroid->type){
		case 0:
			break;
		case 1:
			for(int i=0; i<2; i++){
			tempVector.x=(100-(rand()%200))*0.0005;
			tempVector.y=(100-(rand()%200))*0.0005;
			tempObject=new CWorldObject(defaultAsteroid2,tempVector,Random(1,10)*0.01*M_PI/180.,0,1);
			tempObject->center.x=tempObject->CalculateCenter().x;
			tempObject->center.y=tempObject->CalculateCenter().y;
			tempObject->type=2;
//
			tempVector=tempObject->CalculateCenter();
			tempVector.x=-tempVector.x;
			tempVector.y=-tempVector.y;
			Physics->TranslateWorldObject(tempObject, tempVector);
			tempVector.x=asteroid->CalculateCenter().x;
			tempVector.y=asteroid->CalculateCenter().y;
			Physics->TranslateWorldObject(tempObject, tempVector);
//
			tempObject->radius=tempObject->CalculateRadius();
			asteroids->push_back(tempObject);
			}
			points+=20;
			break;
		case 2:
			for(int i=0; i<2; i++){
				tempVector.x=(100-(rand()%200))*0.001;
				tempVector.y=(100-(rand()%200))*0.001;
				tempObject=new CWorldObject(defaultAsteroid3,tempVector,Random(1,10)*0.01*M_PI/180.,0,1);
				tempObject->center.x=tempObject->CalculateCenter().x;
				tempObject->center.y=tempObject->CalculateCenter().y;
				tempObject->type=3;
	//
	//
				tempVector=tempObject->CalculateCenter();
				tempVector.x=-tempVector.x;
				tempVector.y=-tempVector.y;
				Physics->TranslateWorldObject(tempObject, tempVector);
				tempVector.x=asteroid->CalculateCenter().x;
				tempVector.y=asteroid->CalculateCenter().y;
				Physics->TranslateWorldObject(tempObject, tempVector);
//
//			
			tempObject->radius=tempObject->CalculateRadius();
			asteroids->push_back(tempObject);
			}
			points+=50;
			break;
		case 3:
			points+=100;
			break;
		default:
			break;
	}
	//stworzenie nowej asteroidy (tj. dwoch)
}

void CArcanoidApp::CheckCollisionWithBullets(void){
	Vector tempvector(0,0);
for(WorldObjectsList::iterator bull_iter=bullets->begin(); bull_iter != bullets->end(); bull_iter++) {
	for(WorldObjectsList::iterator iter=asteroids->begin(); iter != asteroids->end(); iter++) {
			//wystaje?
	//sprawdŸ kolizjê(*bull_iter)
		if((*iter)->isCollidable){
	if(Physics->CheckCollision((*bull_iter),(*iter))){
			CollisionWithBullet(*iter,*bull_iter);
	} 
	else {
		switch(Physics->IsStickingOut((*iter)->polygon)){
		case 0:
			break;
		case 1:
					tempvector.y=0;
					tempvector.x+=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision((*bull_iter),(*iter))){
						CollisionWithBullet(*iter,*bull_iter);
					}
					tempvector.x-=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		case 2:
					tempvector.y=0;
					tempvector.x-=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision((*bull_iter),(*iter))){
						CollisionWithBullet(*iter,*bull_iter);
					}
					tempvector.x+=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		case 3:		
					tempvector.x=0;
					tempvector.y+=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision((*bull_iter),(*iter))){
						CollisionWithBullet(*iter,*bull_iter);
					}
					tempvector.y-=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		case 4:	
					tempvector.x=0;
					tempvector.y-=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision((*bull_iter),(*iter))){
						CollisionWithBullet(*iter,*bull_iter);
					}
					tempvector.y+=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		default:
				
			break;
		}
	(*iter)->center.x=(*iter)->CalculateCenter().x;
	(*iter)->center.y=(*iter)->CalculateCenter().y;
	(*iter)->CalculateRadius();
	}
	}
	}
}
}

void CArcanoidApp::OutOfBoundsCheck(void){
//Za bardzo w gore/prawo/lewo/dol?? Przestawianie z powrotem
	Vector tempvector(0,0);
	Vector centerpoint(0,0);
	for(WorldObjectsList::iterator iter=asteroids->begin(); iter != asteroids->end(); iter++) {
		centerpoint = (*iter)->CalculateCenter();
		tempvector.x=0;
		tempvector.y=0;

		if(centerpoint.x < 0 ){
			tempvector.x=width;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
		if (centerpoint.x > width ){
			tempvector.x=-width;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
		if(centerpoint.y < 0 ){
			tempvector.y=height;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
		if(centerpoint.y > height){
			tempvector.y=-height;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
	}

	for(WorldObjectsList::iterator iter=bullets->begin(); iter != bullets->end(); iter++) {
		centerpoint = (*iter)->CalculateCenter();
		tempvector.x=0;
		tempvector.y=0;

		if(centerpoint.x < 0 ){
			tempvector.x=width;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
		if (centerpoint.x > width ){
			tempvector.x=-width;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
		if(centerpoint.y < 0 ){
			tempvector.y=height;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
		if(centerpoint.y > height){
			tempvector.y=-height;
			Physics->TranslateWorldObject((*iter), tempvector);
		}
	}


		if(Ship->center.x < 0 ){
			tempvector.y=0;
			tempvector.x=width;
			Physics->TranslateWorldObject(Ship, tempvector);
		}
		if (Ship->center.x > width ){
			tempvector.y=0;
			tempvector.x=-width;
			Physics->TranslateWorldObject(Ship, tempvector);
		}
		if(Ship->center.y < 0 ){
			tempvector.x=0;
			tempvector.y=height;
			Physics->TranslateWorldObject(Ship, tempvector);
		}
		if(Ship->center.y > height){
			tempvector.x=0;
			tempvector.y=-height;
			Physics->TranslateWorldObject(Ship, tempvector);
		}
}

void CArcanoidApp::DrawParticles(void){
	for(WorldObjectsList::iterator iter=particles->begin(); iter != particles->end(); iter++) {
		Renderer->PolygonFilled((*iter)->polygon, (*iter)->type);
		(*iter)->CalculateRadius();
		if(debug) Renderer->Circle((*iter)->center, (*iter)->radius,(*iter)->type);
	}
}

void CArcanoidApp::DrawAsteroids(void){
	for(WorldObjectsList::iterator iter=asteroids->begin(); iter != asteroids->end(); iter++) {
		Renderer->PolygonFilled((*iter)->polygon, asteroid_color);
		if(debug)Renderer->Circle((*iter)->center, (*iter)->radius, asteroid_color);
	}
}

void CArcanoidApp::DrawBullets(void){
	for(WorldObjectsList::iterator iter=bullets->begin(); iter != bullets->end(); iter++) {
		Renderer->PolygonFilled((*iter)->polygon, bullet_color);
	}
}

void CArcanoidApp::DrawShip(void){
	Renderer->PolygonFilled(Ship->polygon ,ship_color);
	if(debug)Renderer->Circle(Ship->center, Ship->radius, ship_color);
	
}

void CArcanoidApp::NextLevel(){
	//czyszczenie list itp.
	//nowy statek
	level++;
	CenterShip();
	GenerateAsteroids(level+3);
	//zostaja punkty i zycia i inf o levelu
}

void CArcanoidApp::CheckCollisionWithShip(){
	Vector tempvector(0,0);

for(WorldObjectsList::iterator iter=asteroids->begin(); iter != asteroids->end(); iter++) {
			//wystaje?
	//sprawdŸ kolizjê
	(*iter)->radius=(*iter)->CalculateRadius();
	(*iter)->center=(*iter)->CalculateCenter();
	if((*iter)->isCollidable){
	if(Physics->CheckCollision(Ship,(*iter))) {
		CollisionWithShip((*iter));
		//Renderer->Circle((*iter)->center, (*iter)->radius, 0x0000ff);
	}
	else{
	switch(Physics->IsStickingOut((*iter)->polygon)){
		case 0:
			break;
		case 1:
					tempvector.y=0;
					tempvector.x+=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision(Ship,(*iter))) {
						CollisionWithShip((*iter));
						//Renderer->Circle((*iter)->center, (*iter)->radius, 0x0000ff);
					}
					tempvector.x-=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		case 2:
					tempvector.y=0;
					tempvector.x-=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision(Ship,(*iter))) {
						CollisionWithShip((*iter));
						//Renderer->Circle((*iter)->center, (*iter)->radius, 0x0000ff);
					}
					tempvector.x+=width;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		case 3:		
					tempvector.x=0;
					tempvector.y+=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision(Ship,(*iter))) {
						CollisionWithShip((*iter));
						//Renderer->Circle((*iter)->center, (*iter)->radius, 0x0000ff);
					}
					tempvector.y-=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		case 4:	
					tempvector.x=0;
					tempvector.y-=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
					if(Physics->CheckCollision(Ship,(*iter))) {
						CollisionWithShip((*iter));
						//Renderer->Circle((*iter)->center, (*iter)->radius, 0x0000ff);
					}
					tempvector.y+=height;
					Physics->TranslatePolygon((*iter)->polygon, tempvector);
			break;
		}
	
	(*iter)->center.x=(*iter)->CalculateCenter().x;
	(*iter)->center.y=(*iter)->CalculateCenter().y;
	}
	}
}
}
void CArcanoidApp::CollisionWithShip(CWorldObject *collider){
	CWorldObject *temp=collider;
	//Renderer->Circle(Ship->center, Ship->radius, 0x0000ff);
	if(lifes>0) lifes--;
	MakeBoomEffect(Ship->center, 50, ship_color);
	CenterShip();

}

void CArcanoidApp::MakeBoomEffect(Vector point, int sizeOfBoom, int color){
	Vector tempVector(0,0);
	CWorldObject *tempObject;
	srand(time(NULL));
	for(int i=0; i<sizeOfBoom; i++){
		tempVector.x=(150-(rand()%200))*0.003;
		tempVector.y=(150-(rand()%200))*0.003;

		tempObject=new CWorldObject(defaultParticle,tempVector,Random(1,10)*0.01*M_PI/180.,0,1);	
		tempObject->center.x=tempObject->CalculateCenter().x;
		tempObject->center.y=tempObject->CalculateCenter().y;

		tempVector.x=-tempObject->center.x+point.x;
		tempVector.y=-tempObject->center.y+point.y;

		Physics->TranslateWorldObject(tempObject, tempVector);
		tempObject->center.x=tempObject->CalculateCenter().x;
		tempObject->center.y=tempObject->CalculateCenter().y;
		tempObject->type=color;
		tempObject->angvelocity=(rand()%10)*0.05;
		particles->push_back(tempObject);
	}
}

void CArcanoidApp::CheckAndUpdateParticles(){
	CWorldObject* temp;
	Vector tempVector(0,0);
		for(WorldObjectsList::iterator iter=particles->begin(); iter != particles->end();) {
//particle z silnika ¿yj¹ krócej
			if((*iter)->type==engine_color){
				if((*iter)->liveTime >50){
					temp=(*iter);
					iter = particles->erase(iter);
					delete temp;
				}
				else {
					(*iter)->liveTime+=deltaTime;
					++iter; 
				}

			}
			else {
				if((*iter)->liveTime >500){
					temp=(*iter);
					iter = particles->erase(iter);
					delete temp;
				}
				else {
					(*iter)->liveTime+=deltaTime;
					++iter; 
				}
			}
		}

		for(WorldObjectsList::iterator iter=particles->begin(); iter != particles->end(); iter++) {
			//polozenie
			tempVector.x=(*iter)->velocity.x*deltaTime;
			tempVector.y=(*iter)->velocity.y*deltaTime;
			Physics->TranslateWorldObject((*iter),tempVector);
			(*iter)->center.x=(*iter)->CalculateCenter().x;
			(*iter)->center.y=(*iter)->CalculateCenter().y;
			Physics->RotatePolygon((*iter)->polygon,(*iter)->center, ((*iter)->angvelocity*deltaTime));
		}
}

void CArcanoidApp::CollisionWithShip(void){
//	Renderer->Circle(Ship->center, Ship->radius, 0x0000ff);
	if(lifes>0) lifes--;
	CenterShip();
}

void CArcanoidApp::CenterShip(){
		Vector tempVector(0,0);
			tempVector=Ship->center;
			tempVector.x=-tempVector.x;
			tempVector.y=-tempVector.y;
			Physics->TranslateWorldObject(Ship, tempVector);
			tempVector.x=width/2;
			tempVector.y=height/2;
			Physics->TranslateWorldObject(Ship, tempVector);
			Ship->velocity.x=0;
			Ship->velocity.y=0;
			Physics->RotatePolygon(Ship->polygon, Ship->center, -(Ship->actual_angle));
			Ship->actual_angle=0;

}


int CArcanoidApp::Random(int min, int max){
	return (rand()%(max-min))+min; 
}

void CArcanoidApp::HandleEvents(){
//zale¿nie od czasu -> przyspieszanie itp. itd.?
	int actual_time=SDL_GetTicks();
	Vector tempVector(0,0);
	CWorldObject *tempObject;

//Skret
	if((isTurning_right==0) && (isTurning_left==0)) Ship->angvelocity=0;
	if(actual_time-TimeOfLastTurning>20){
		if(isTurning_left){
			//skret w lewo
			Ship->angvelocity=-0.25*M_PI/180.;
			TimeOfLastTurning=actual_time;
		}
		if(isTurning_right){
			//skret w prawo
			Ship->angvelocity=0.25*M_PI/180.;
			TimeOfLastTurning=actual_time;
		}
	}

//Strzal
	if((actual_time-TimeOfLastShoot>200)&&(bullets->size()<5)){
		if(isShooting){
			//strzelanie
			tempVector.x=Ship->velocity.x+sin(Ship->actual_angle) * 0.5;
			tempVector.y=Ship->velocity.y-cos(Ship->actual_angle) * 0.5;
			tempObject=new CWorldObject(defaultBullet,tempVector,0,1,1);
			tempVector=tempObject->CalculateCenter();
			tempVector.x=-tempVector.x;
			tempVector.y=-tempVector.y;
			Physics->TranslateWorldObject(tempObject, tempVector);
			tempVector.x=Ship->center.x;
			tempVector.y=Ship->center.y;
			Physics->TranslateWorldObject(tempObject, tempVector);
			Physics->RotatePolygon(tempObject->polygon, tempObject->CalculateCenter(), Ship->actual_angle);
			bullets->push_back(tempObject);
			TimeOfLastShoot=actual_time;
		}
	}
	
//przyspieszanie
	double normalVelocity=sqrt((Ship->velocity.x * Ship->velocity.x)+(Ship->velocity.y * Ship->velocity.y));
	if(actual_time-TimeOfLastAccelerating>15){
		if((isAccelerating)&&(normalVelocity<shipMaxSpeed)){
			//przyspieszanie
			Ship->velocity.x+=sin(Ship->actual_angle) * 0.008; //przyspieszenie
			Ship->velocity.y-=cos(Ship->actual_angle) * 0.008; //przyspieszenie
			TimeOfLastAccelerating=actual_time;
		}
	}
	
	if(isAccelerating){
		if(actual_time-TimeOfLastEngineParticle>25){	
			Vector tempVector(0,0);
			CWorldObject *tempObject;
			srand(time(NULL));
			
			tempVector.x=(-sin(Ship->actual_angle) * (rand()%100) * 0.005);//+Ship->velocity.x;
			tempVector.y=(cos(Ship->actual_angle) * (rand()%100) * 0.005);//+Ship->velocity.y;

			tempObject=new CWorldObject(defaultParticle,tempVector,Random(1,10)*0.01*M_PI/180.,0,1);	
			tempObject->center.x=tempObject->CalculateCenter().x;
			tempObject->center.y=tempObject->CalculateCenter().y;

			tempVector.x=-tempObject->center.x+Ship->center.x-sin(Ship->actual_angle)*5;
			tempVector.y=-tempObject->center.y+Ship->center.y+cos(Ship->actual_angle)*5;

			Physics->TranslateWorldObject(tempObject, tempVector);
			tempObject->center.x=tempObject->CalculateCenter().x;
			tempObject->center.y=tempObject->CalculateCenter().y;
			tempObject->type=engine_color;
		//	tempObject->angvelocity=(rand()%10)*0.05;
			particles->push_back(tempObject);
			TimeOfLastEngineParticle=actual_time;
		}
	}


}

void CArcanoidApp::Fail(){
//Renderer->Fail()?? napisy? cos?
	
//SDL_Quit();
}
void CArcanoidApp::GenerateAsteroids(int numberOfAsteroids){
	Vector tempVector(0.005,0.005);
	CWorldObject *tempObject;
	Vector screencenter(((double)width/2.),((double)height/2.));
	int radius= (3.*(double)height)/8.;
	srand ( time(NULL) );
	double tempdegs;
	for(int i=0; i<numberOfAsteroids; i++){
		tempVector.x=(100-(rand()%200))*0.0001;
		tempVector.y=(100-(rand()%200))*0.0001;
		tempObject=new CWorldObject(defaultAsteroid,tempVector,Random(1,10)*0.01*M_PI/180.,0,1);
	
			tempVector=tempObject->CalculateCenter();
			tempVector.x=-tempVector.x;
			tempVector.y=-tempVector.y;
			Physics->TranslateWorldObject(tempObject, tempVector);
			tempdegs=(((360./(double)numberOfAsteroids)*(double)i*M_PI)/180);
			tempVector.x=screencenter.x+(cos(tempdegs)*(double)radius);
			tempVector.y=screencenter.y+(sin(tempdegs)*(double)radius);
			Physics->TranslateWorldObject(tempObject, tempVector);
			Physics->RotatePolygon(tempObject->polygon, tempObject->CalculateCenter(), Ship->actual_angle);

		tempObject->type=1;
		asteroids->push_back(tempObject);
	}
}

void CArcanoidApp::MainLoop(){
	int quit = 0;
	int actualfps = 0;
	char temp[11]; 

	while(!quit){
		Renderer->ClearScreenToColor(0x000000);
		CollisionsCheck(); //kolizje
		OutOfBoundsCheck(); //wypadanie za ekran
		
		UpdateTime(); //update deltaT
		UpdateObjectsStates(); //Symulacja sil
		
		CheckAndUpdateParticles();
		//rysowanie
		DrawParticles();
		DrawAsteroids();
		DrawBullets();
		DrawShip();

		//Rysowanie górnego podsumowania
		Renderer->DrawText("Score:",10,10);
		sprintf(temp, "%lu", points);
		Renderer->DrawText(temp, 105,10);
		
		sprintf(temp, "%lu", lifes);
		Renderer->DrawText("Lives:",200,10);
		Renderer->DrawText(temp, 280,10);

		sprintf(temp, "%lu", level);
		Renderer->DrawText("Level:",width-150,10);
		Renderer->DrawText(temp, width-60,10);
		if(debug){
			sprintf(temp, "%lu", fps);
			Renderer->DrawText("FPS:",10,height-40);
			Renderer->DrawText(temp, 80,height-40);
		}
		//refresh - double buffering
		Renderer->FlipScreen(); 		
		//klawisze sie zmienily?
		quit=CheckKeysState();	
		//obsluga klawiszy
		HandleEvents();	

		//sekundnik (zerowany co sekunde - licznik FPS)
		if(asteroids->size()==0) {
			NextLevel();
		}

		if(lifes==0){
			Fail();
			quit=1;
		}

		//
		if(secondHand>=1000) {
			secondHand=0; 
			fps=actualfps;
			actualfps=0;
		}
		else actualfps++;
		//Sleep(5);
	}
}
int CArcanoidApp::CheckKeysState(void){
	int out=0;

	while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					//switch? accelt,turning,shooting
					switch(event.key.keysym.sym){
						case SDLK_ESCAPE:
							out = 1;	
							break;
						case SDLK_UP:
							isAccelerating=1;
							break;
						case SDLK_DOWN:
							break;
						case SDLK_LEFT:
							isTurning_left=1;
							break;
						case SDLK_RIGHT:
							isTurning_right=1;
							break;
						case SDLK_b: 
							isShooting=1;
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym){
						case SDLK_UP:
							isAccelerating=0;
							break;
						case SDLK_DOWN:
							break;
						case SDLK_LEFT:
							isTurning_left=0;
							break;
						case SDLK_RIGHT:
							isTurning_right=0;
							break;
						case SDLK_b:
							isShooting=0;
							break;
					}
					break;
				case SDL_QUIT:
					out = 1;
					break;
			}
		}
	return out;
}

void CArcanoidApp::Quit(){
	SDL_Quit();
}

int CArcanoidApp::Run(void){ 
	
	if(Initialize()) return 1;
	Renderer->InitFont("font.ttf",24);
	Ship = new CShip(defaultSpaceShip,0,0,0,0,1);
	Ship->center.x=defaultShipCenter.x;
	Ship->center.y=defaultShipCenter.y;
	
	//GenerateAsteroids(15);
	
	Physics = new CPhysicsSimulator(width,height);
	lastTime = SDL_GetTicks();//zebranie pierwszego czasu
	TimeOfLastShoot=lastTime;
	TimeOfLastAccelerating=lastTime;
	TimeOfLastTurning=lastTime;
	TimeOfLastEngineParticle=lastTime;
	FractionTimer=lastTime;

	MainLoop();
	Quit();

	return 0;
}

int CArcanoidApp::LoadConfigurationFromFile(char* filename){
	//loader
	int lwierzcholkow=0;
	int wierzcholekx=0;
	int wierzcholeky=0;
	int kolor;
	FILE *pFile;
	pFile = fopen(filename , "r");
	if (pFile == NULL) printf("Error opening file");
	else{
		defaultSpaceShip->clear();
		defaultAsteroid->clear();
		defaultAsteroid2->clear();
		defaultAsteroid3->clear();
		//statek
		fscanf(pFile,"%d", &lwierzcholkow);
			for(int i=0; i<lwierzcholkow;i++){
				fscanf(pFile,"%d", &wierzcholekx);
				fscanf(pFile,"%d", &wierzcholeky);
				defaultSpaceShip->push_back(new Vector(wierzcholekx,wierzcholeky));
			}
			defaultSpaceShip->push_back(NULL);
//srodek statku
			fscanf(pFile,"%d", &wierzcholekx);
			fscanf(pFile,"%d", &wierzcholeky);
			defaultShipCenter.x=wierzcholekx;
			defaultShipCenter.y=wierzcholeky;
//kolorki
			fscanf(pFile,"%x", &kolor);
			ship_color=kolor;
			fscanf(pFile,"%x", &kolor);
			engine_color=kolor;
		//
		//asteroida1
		fscanf(pFile,"%d", &lwierzcholkow);
			for(int i=0; i<lwierzcholkow;i++){
				fscanf(pFile,"%d", &wierzcholekx);
				fscanf(pFile,"%d", &wierzcholeky);
				defaultAsteroid->push_back(new Vector(wierzcholekx,wierzcholeky));
			}
			defaultAsteroid->push_back(NULL);
		//asteroida2
		fscanf(pFile,"%d", &lwierzcholkow);
			for(int i=0; i<lwierzcholkow;i++){
				fscanf(pFile,"%d", &wierzcholekx);
				fscanf(pFile,"%d", &wierzcholeky);
				defaultAsteroid2->push_back(new Vector(wierzcholekx,wierzcholeky));
			}
			defaultAsteroid2->push_back(NULL);
		//asteroida3
		fscanf(pFile,"%d", &lwierzcholkow);
			for(int i=0; i<lwierzcholkow;i++){
				fscanf(pFile,"%d", &wierzcholekx);
				fscanf(pFile,"%d", &wierzcholeky);
				defaultAsteroid3->push_back(new Vector(wierzcholekx,wierzcholeky));
			}
			defaultAsteroid3->push_back(NULL);
		
		//kolor asteroid
		fscanf(pFile,"%x", &kolor);
		asteroid_color=kolor;
		//kolor naboi
		fscanf(pFile,"%x", &kolor);
		bullet_color=kolor;
		//debug?
		fscanf(pFile,"%d", &wierzcholekx);
		debug=wierzcholekx;
		fclose(pFile);
	}
	
	return 0;
}

CArcanoidApp::~CArcanoidApp(void)
{
	//czyszczenie list jeszcze tutaj
	for(WorldObjectsList::iterator iter=asteroids->begin(); iter != asteroids->end();) {
		CWorldObject* temp = *iter;
		iter = asteroids->erase(iter);
		delete temp;
	}
	for(WorldObjectsList::iterator iter=bullets->begin(); iter != bullets->end();) {
		CWorldObject* temp = *iter;
		iter = bullets->erase(iter);
		delete temp;
	}

	delete asteroids;
	delete bullets;

	delete Renderer;
	delete Ship;
	delete Physics;
}
