#include "PhysicsSimulator.h"

int CPhysicsSimulator::CheckVectorCollision(Vector start_1, Vector end_1, Vector start_2, Vector end_2){
   int a,b,c;
   int k1,k2;

   
	// prosta na której le¿y wektor
   a = end_1.y - start_1.y;
   b = -(end_1.x - start_1.x);
   c = -a * start_1.x - b * start_1.y;

     /* Obliczenie odleglosci punktow poczatkowego i koncowego wektora 2 od prostej, na 
      ktorej lezy drugi wektor. Zeby wektory sie przecinaly, to punkty te' musza znajdowac sie po 
      obu stronach tej prostej.  */

   //leza po tej samej stronie  - nie przecinaja
   k1 = a * start_2.x + b * start_2.y + c; 
   k2 = a * end_2.x+ b * end_2.y + c;  
   if ((k1 > 0 && k2 > 0) || (k1 < 0 && k2 < 0)) return 0;    


//analogicznie teraz 1 od 2
   a = end_2.y - start_2.y;
   b = -(end_2.x- start_2.x);
   c = -a * start_2.x - b * start_2.y;

   k1 = a * start_1.x + b * start_1.y + c;   
   k2 = a * end_1.x + b * end_1.y + c;  

   //leza po jednej stronie - nie przecinaja
   if ((k1 > 0 && k2 > 0) || (k1 < 0 && k2 < 0)) return 0;    


	//leza oba po innych stronach - przecinaja sie
   return 1;

}
inline double CPhysicsSimulator::DistanceBetweenPoints(Vector* v1, Vector* v2){
	return sqrt(((v2->x-v1->x)*(v2->x-v1->x)) + ((v2->y-v1->y)*(v2->y-v1->y)));
		
}

int CPhysicsSimulator::CheckCircleCollisions(Vector center1, double radius1, Vector center2, double radius2){

	if(DistanceBetweenPoints(&center1, &center2)<=radius1+radius2) return 1;
	else return 0;

}

int CPhysicsSimulator::IsStickingOut(PolygonVertices* polygon){
	for(int i=0; i<polygon->size();i++){
		if(((*polygon)[i])->x<0) return 1;//lewo
		else if(((*polygon)[i])->x > width) return 2;//prawo
		if(((*polygon)[i])->y<0) return 3;//gora
		else if(((*polygon)[i])->y > height) return 4;//dol
	}
	return 0;
}

int CPhysicsSimulator::CheckPolygonsCollisions(PolygonVertices* polygon1, PolygonVertices* polygon2){
	for(int i=0; i<polygon1->size()-1; i++){
		for(int j=0; j<polygon2->size()-1;j++){
			if(CheckVectorCollision(*((*polygon1)[i]), *((*polygon1)[i+1]), *((*polygon2)[j]),*((*polygon2)[j+1]))){
				return 1;
			}
		}
	}
	for(int j=0; j<polygon2->size()-1;j++){
		if(CheckVectorCollision(*((*polygon1)[polygon1->size()-1]), 
								*((*polygon1)[0]), 
								*((*polygon2)[j]),
								*((*polygon2)[j+1]))){
			return 1;
			}
		}

	for(int i=0; i<polygon1->size()-1;i++){
		if(CheckVectorCollision(*((*polygon1)[i]), *((*polygon1)[i+1]), *((*polygon2)[polygon2->size()-1]),*((*polygon2)[0]))){
				return 1;
			}
	}

	return 0;
}

int CPhysicsSimulator::CheckCollision(CWorldObject* first, CWorldObject* second){

	//Chyba jednak nie...
	first->center=first->CalculateCenter();
	first->CalculateRadius();
	second->center=second->CalculateCenter();
	second->CalculateRadius();

	//A to tak:
	if(CheckCircleCollisions(first->center, first->radius, second->center, second->radius)){
		if(CheckPolygonsCollisions(first->polygon, second->polygon)) return 1;
		else return 0;
	}
	else return 0;

}
void CPhysicsSimulator::TranslatePoint(Vector* point, Vector vec2){
	point->x+=vec2.x;
	point->y+=vec2.y;
}

void CPhysicsSimulator::RotatePoint(Vector* point, Vector point_zero, double angle){
	//x' = (x-x0)cosalfa - (y-y0)sinalfa + x0
	//y' = (x-x0)sinalfa + (y-y0)cosalfa + y0
	double x=point->x;
	double y=point->y;
	double sinus=sin(angle);
	double cosinus=cos(angle);
	point->x=(x - point_zero.x)*cosinus - (y - point_zero.y)*sinus + point_zero.x;
	point->y=(x - point_zero.x)*sinus + (y - point_zero.y)*cosinus + point_zero.y;

}
void CPhysicsSimulator::RotatePolygon(PolygonVertices* polygon, Vector center, double angle){
	for(int i=0; i<polygon->size(); i++){
		RotatePoint((*polygon)[i], center, angle);
	}
}

void CPhysicsSimulator::TranslatePolygon(PolygonVertices* polygon, Vector vec2){
	for(int i=0; i<polygon->size(); i++){
		TranslatePoint((*polygon)[i], vec2);
	}
}

void CPhysicsSimulator::TranslateWorldObject(CWorldObject* worldobject, Vector vec2){
	for(int i=0; i<worldobject->polygon->size(); i++){
		TranslatePoint((*(worldobject->polygon))[i], vec2);
	}
	TranslatePoint(&(worldobject->center), vec2);
}


int CPhysicsSimulator::UpdateState(CWorldObject* worldobject, int deltaTime){
	Vector temp_deltaTime;
	temp_deltaTime.x=(worldobject->velocity.x * deltaTime * 0.001);
	temp_deltaTime.y=(worldobject->velocity.y * deltaTime * 0.001);
	TranslatePolygon(worldobject->polygon, temp_deltaTime);
	TranslatePoint(&(worldobject->center),temp_deltaTime);
	RotatePolygon(worldobject->polygon, worldobject->center ,worldobject->angvelocity*deltaTime*0.001);
	return 0;
}

CPhysicsSimulator::CPhysicsSimulator(int _width,int _height)
{
	width=_width; 
	height= _height;

}

CPhysicsSimulator::~CPhysicsSimulator(void)
{
}
