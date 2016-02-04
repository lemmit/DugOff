#pragma once

/*
Klasa abstrakcyjna opisujaca pojedynczy wpis w bazie
Pozwala na przechowywanie informacji oraz (de)serializacje binarna do tablicy bajtow
*/

class Record
{
public:
	//serializacja
	virtual void copyTo(unsigned char* buf) = 0; 

	/*
	//Przeciazone operatory do porownywania recordow
	virtual bool operator<(Record &rec) = 0;
	virtual bool operator>(Record &rec) = 0;
	virtual bool operator==(Record &rec) = 0;
	*/
	//Wielkosc recordu - w bajtach

	static unsigned int size();
};

