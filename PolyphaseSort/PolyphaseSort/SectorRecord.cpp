#include "StdAfx.h"
#include "SectorRecord.h"

SectorRecord::SectorRecord(void){

}

SectorRecord::SectorRecord(const SectorRecord& rec){
	this->_alfa=((SectorRecord&)rec).getAlfa();
	this->_r=((SectorRecord&)rec).getR();
}

SectorRecord::SectorRecord(double r, double alfa) : _r(r), _alfa(alfa) {
}

/* Deserializacja  */
SectorRecord::SectorRecord(unsigned char *buf){
	if(buf!=NULL){
		//debug
		/*
		printf("Record: ");
		for(int i=0; i<17; i++) printf("%d ", (int)buf[i]);
		printf("\n");
		*/
		double* src = ((double*)buf);
		_r = src[0];
		_alfa = src[1];
	}
}

void SectorRecord::copyTo(unsigned char* buf){
	if(buf!=NULL){
		double* dbuf = (double*)buf;
		dbuf[0] = getR();
		dbuf[1] = getAlfa();
	}
}

/* Wielkosc rekordu w bajtach */
unsigned int SectorRecord::size(){
	return sizeof(double)*2;
}

/* Porownywania */
bool SectorRecord::operator<(SectorRecord &rec){
	return this->getR()	< rec.getR();
}

bool SectorRecord::operator>(SectorRecord &rec){
	return this->getR()	> rec.getR();
}

bool SectorRecord::operator==(SectorRecord &rec){
	return this->getR() == rec.getR();
}

double SectorRecord::getR(){
	return _r;
}

double SectorRecord::getAlfa(){
	return _alfa;
}

void SectorRecord::print(){
	printf("%f %f ", getR(), getAlfa());
}

void SectorRecord::println(){
	printf("%f %f \n", getR(), getAlfa());
}

SectorRecord::~SectorRecord(void)
{
}
