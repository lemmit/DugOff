#include "WorldObject.h"

CWorldObject::CWorldObject(){
	polygon = new PolygonVertices;
}

CWorldObject::CWorldObject(PolygonVertices* _polygon, Vector _velocity, 
						   double _angvelocity, int _isBullet, int _isCollidable){
	polygon = new PolygonVertices;
SetObject(_polygon,_velocity,_angvelocity,_isBullet,_isCollidable);
isDying=0;
liveTime=0;
}

CWorldObject::CWorldObject(PolygonVertices* _polygon, double velocity_x, 
						   double velocity_y, double _angvelocity, int _isBullet, int _isCollidable){
	polygon = new PolygonVertices;
SetObject(_polygon,velocity_x,velocity_y,_angvelocity,_isBullet,_isCollidable);
isDying=0;
liveTime=0;
}


void CWorldObject::SetObject(PolygonVertices* _polygon, Vector _velocity, 
							 double _angvelocity, int _isBullet, int _isCollidable){


	Vector* tempVector;
	for(int i=0; i<_polygon->size()-1; i++){
		tempVector=new Vector(((*_polygon)[i])->x, ((*_polygon)[i])->y);
		polygon->push_back(tempVector);
	}

	velocity.x=_velocity.x;
	velocity.y=_velocity.y;
	angvelocity=_angvelocity;
	isBullet= _isBullet;
	isCollidable= _isCollidable;
	liveTime=0;
}

void CWorldObject::SetObject(PolygonVertices* _polygon, 
							 double velocity_x, double velocity_y, double _angvelocity, int _isBullet, int _isCollidable){
	
	Vector* tempVector;
	for(int i=0; i<_polygon->size()-1; i++){
		tempVector=new Vector(((*_polygon)[i])->x, ((*_polygon)[i])->y);
		polygon->push_back(tempVector);
	}
	velocity.x=velocity_x;
	velocity.y=velocity_y;
	angvelocity=_angvelocity;
	isBullet= _isBullet;
	isCollidable= _isCollidable;
	liveTime=0;
}

Vector CWorldObject::CalculateCenter(){
	int maxX=0,minX=0,maxY=0,minY=0;
	Vector centerpoint;
	if(polygon->size()>2) {
		maxX=((*polygon)[0])->x;
		minX=((*polygon)[0])->x;
		maxY=((*polygon)[0])->y;
		minY=((*polygon)[0])->y;
	}

	for(int i=0; i<polygon->size();i++){
		if(maxX<((*polygon)[i])->x) maxX=((*polygon)[i])->x;
		if(maxY<((*polygon)[i])->y) maxY=((*polygon)[i])->y;
		if(minX>((*polygon)[i])->x) minX=((*polygon)[i])->x;
		if(minY>((*polygon)[i])->y) minY=((*polygon)[i])->y;
	}
	centerpoint.x=(maxX+minX)/2;
	centerpoint.y=(maxY+minY)/2;
	return centerpoint;
}

double CWorldObject::CalculateRadius(){
	double distance=0;
	int temp=0;

	CalculateCenter();
	//getdistancefrom center to every verticle.
	for(int i=0; i<polygon->size();i++){
		temp=sqrt( (((*polygon)[i])->x - center.x)*(((*polygon)[i])->x - center.x)+ (((*polygon)[i])->y - center.y)*(((*polygon)[i])->y - center.y));
		if(temp>distance) distance=temp;
	}
	radius=distance;
	return distance;
}

void CWorldObject::Die(){
	isDying=1;
}

CWorldObject::~CWorldObject(void)
{
	for(PolygonVertices::iterator iter=polygon->begin(); iter != polygon->end();) {
		Vector* temp = *iter;
		iter = polygon->erase(iter);
		delete temp;
	}

	delete polygon;
}
