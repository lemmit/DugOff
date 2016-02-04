#pragma once
#include "record.h"
/*
Wycinek kolowy - posiada promien i kat
Porownywanie umozliwia uporzadkowanie w/g pola
( Zadanie nr 26 )

Klasa implementujaca Record
*/
class SectorRecord :
	public Record
{
private:
	//promien
	double _r;

	//kat
	double _alfa;
public:
	SectorRecord(void);

	SectorRecord(const SectorRecord &rec);

	SectorRecord(double r, double alfa);
	//deserializacja
	SectorRecord(unsigned char* buf);

	//serializacja
	void copyTo(unsigned char* buf);

	//wypisz wartosci na stdout
	void print();
	void println();

	//Przeciazone operatory do porownywania wycinkow kola
	bool operator<(SectorRecord &rec);
	bool operator>(SectorRecord &rec);
	bool operator==(SectorRecord &rec);
	
	double getR();
	double getAlfa();

	//Wielkosc recordu - w bajtach
	static unsigned int size();

	~SectorRecord(void);
};

