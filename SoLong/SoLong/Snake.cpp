#include "StdAfx.h"
#include "Snake.h"

CSnake::CSnake(void)
{
	SnakeElements= new std::list<CSnakeElement*>;
}
int CSnake::RenderSnake(int TabToRender[80][50]){
	for(std::list<CSnakeElement*>::iterator iter=SnakeElements->begin(); iter != SnakeElements->end(); ++iter )
	{
		if (((*iter)->isHead)==SNAKEHEAD) {
			TabToRender[(*iter)->x][(*iter)->y]=SNAKEHEAD;
		}
		else TabToRender[(*iter)->x][(*iter)->y]=SNAKEBODY;
	}
	return 0;
}
int CSnake::MoveUp(int TabToRender[80][50]){
	CSnakeElement* SnakeFirstElement=NULL;
	CSnakeElement* SnakeTemporaryElement = NULL;
	SnakeFirstElement=SnakeElements->front();
	if(SnakeFirstElement!=NULL) {
		int temp=TabToRender[SnakeFirstElement->x][(SnakeFirstElement->y)-1];

		switch(temp){
		case SNAKEBODY:
			if(HeadDuration==DOWN) return 0;
			else return SNAKEBODY;
			break;
		case SNAKEHEAD:
			return SNAKEHEAD;
			break;
		case WALL:
			break;
		case TREASURE:
				HeadDuration=UP;
				//Powiekszenie snejka
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement= new CSnakeElement((SnakeFirstElement->x),(SnakeFirstElement->y)-1,SNAKEHEAD);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
			return TREASURE;
			break;
		default:
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement=SnakeElements->back();
				SnakeTemporaryElement->isHead=SNAKEHEAD;
				SnakeTemporaryElement->y=(SnakeFirstElement->y)-1;
				SnakeTemporaryElement->x=(SnakeFirstElement->x);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
				SnakeElements->pop_back();
				HeadDuration=UP;
			return temp;
			break;
		}
	}
}
int CSnake::MoveDown(int TabToRender[80][50]){
	CSnakeElement* SnakeFirstElement=NULL;
	CSnakeElement* SnakeTemporaryElement = NULL;
	SnakeFirstElement=SnakeElements->front();
	if(SnakeFirstElement!=NULL) {
		int temp=TabToRender[SnakeFirstElement->x][(SnakeFirstElement->y)+1];

		switch(temp){
		case SNAKEBODY:
			if(HeadDuration==UP) return 0;
			else return SNAKEBODY;
			break;
		case SNAKEHEAD:
			return SNAKEHEAD;
			break;
		case WALL:
			break;
		case TREASURE:
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement= new CSnakeElement((SnakeFirstElement->x),(SnakeFirstElement->y)+1,SNAKEHEAD);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
				HeadDuration=DOWN;
			return TREASURE;
			break;
		default:
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement=SnakeElements->back();
				SnakeTemporaryElement->isHead=SNAKEHEAD;
				SnakeTemporaryElement->y=(SnakeFirstElement->y)+1;
				SnakeTemporaryElement->x=(SnakeFirstElement->x);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
				SnakeElements->pop_back();
				HeadDuration=DOWN;
			return temp;
			break;
		}
	}
}
int CSnake::MoveRight(int TabToRender[80][50]){
	CSnakeElement* SnakeFirstElement=NULL;
	CSnakeElement* SnakeTemporaryElement = NULL;
	SnakeFirstElement=SnakeElements->front();
	if(SnakeFirstElement!=NULL) {
		int temp=TabToRender[(SnakeFirstElement->x)+1][SnakeFirstElement->y];

		switch(temp){
		case SNAKEBODY:
			if(HeadDuration==LEFT) return 0;
			else return SNAKEBODY;
			break;
		case SNAKEHEAD:
			return SNAKEHEAD;
			break;
		case WALL:
			break;
		case TREASURE:
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement= new CSnakeElement((SnakeFirstElement->x)+1,(SnakeFirstElement->y),SNAKEHEAD);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
				HeadDuration=RIGHT;
			return TREASURE;
			break;
		default:
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement=SnakeElements->back();
				SnakeTemporaryElement->isHead=SNAKEHEAD;
				SnakeTemporaryElement->x=(SnakeFirstElement->x)+1;
				SnakeTemporaryElement->y=(SnakeFirstElement->y);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
				SnakeElements->pop_back();
				HeadDuration=RIGHT;
			return temp;
			break;
		}
	}
}
int CSnake::MoveLeft(int TabToRender[80][50]){
	CSnakeElement* SnakeFirstElement=NULL;
	CSnakeElement* SnakeTemporaryElement = NULL;
	SnakeFirstElement=SnakeElements->front();
	if(SnakeFirstElement!=NULL) {
		int temp=TabToRender[(SnakeFirstElement->x)-1][SnakeFirstElement->y];

		switch(temp){
		case SNAKEBODY:
			if(HeadDuration==RIGHT) return 0;
			else return SNAKEBODY;
			break;
		case SNAKEHEAD:
			return SNAKEHEAD;
			break;
		case WALL:
			return 0;
			break;
		case TREASURE:
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement= new CSnakeElement((SnakeFirstElement->x)-1,(SnakeFirstElement->y),SNAKEHEAD);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
				HeadDuration=LEFT;
			return TREASURE;
			break;
		default:
				SnakeFirstElement->isHead=SNAKEBODY;
				SnakeTemporaryElement=SnakeElements->back();
				SnakeTemporaryElement->isHead=SNAKEHEAD;
				SnakeTemporaryElement->x=(SnakeFirstElement->x)-1;
				SnakeTemporaryElement->y=(SnakeFirstElement->y);
				SnakesHead=SnakeTemporaryElement;
				SnakeElements->push_front(SnakeTemporaryElement);
				SnakeElements->pop_back();
				HeadDuration=LEFT;
			return temp;
			break;
		}
	}
}

int CSnake::Move(int TabToRender[80][50]){
	
	switch(HeadDuration){
	case UP:
		return(MoveUp(TabToRender));
		break;
	case DOWN:
		return(MoveDown(TabToRender));
		break;
	case RIGHT:
		return(MoveRight(TabToRender));
		break;
	case LEFT:
		return(MoveLeft(TabToRender));
		break;
	}
}

void CSnake::Start(void){
	SnakesHead=new CSnakeElement(10,10,SNAKEHEAD);
	SnakeElements->push_back(SnakesHead);
}

CSnake::~CSnake(void)
{
	for(std::list<CSnakeElement*> ::iterator iter=SnakeElements->begin(); iter != SnakeElements->end();){
		CSnakeElement* temp = *iter;
		iter = SnakeElements->erase(iter);
		delete temp;
		}
	delete SnakeElements;
}
