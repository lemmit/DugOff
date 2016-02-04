#pragma once
#include "Record.h"
#include <string>
#include "SequenceIO.h"
#include <vector>
#include <cstdio>

/*
Klasa zapewniajaca sekwencyjny dostep do rekordów - pisanie oraz czytanie, 
obsluguje stronicowanie(strona o zadanej wielkosci).
Niskopoziomowe operacje pisania/czytania wykonuje za pomoca klasy SequenceIO.
Template specjalizowany klasa dziedzicz¹c¹ po Record.
Znak pomiedzy rekordami oraz konca pliku mozna dowolnie ustawiac.
*/

template <typename T>
class PageIO
{
private:
	//ostatnio zwrocony/modyfikowany rekord
	T _lastRecord; 

	//wielkosc strony
	int _pageSize; 
	
	//rozdzielacz miedzy rekordami
	char _recordDelimiter;

	//znak konca danych
	char _EOFsign;

	//wczytywanie/pisanie bajtami
	SequenceIO * _sequenceIO;

	//bufor strony
	unsigned char*  _pageBuffer;

	//tablica z indeksami kolejnych rekordow w buforze strony
	unsigned char** _pagedRecordsBuffer;

	//tryb pisania/czytania
	int _mode;

	//na ktorym rekordzie na stronie sie znajdujemy
	int recordOffset;

	//ile wczytano bajtow z pamieci na aktualna strone
	int hasBeenRead;

	//czy wczytywano dane po zmianie trybu/uruchomieniu/resecie
	bool firstRecord;

	//zapisz strone w pamieci
	void writePage();

	//odczytaj strone z pamieci
	void readPage();

	//czy kolejny wpis jest dostepny na aktualnie wczytanej stronie?
	bool isNextRecordAvaibleOnPage();

	//czy jest to koniec danych?
	bool isEOF();

	//tworzenie bufora pamieci oraz tablicy z adresami rekordów
	bool createPageBuffer(unsigned int pageSize);

	//zapisz to co jest na aktualnej stronie 
	void flush();
public:

	//konstruktor - wiecej przy implementacji
	PageIO(SequenceIO *sequenceIO, int pageSize=512, char recordDelimiter='|', char EOFsign='#');

	//zwroc nastepny rekord
	T getNextRecord(); 

	//zwroc ostatnio zczytywany/wpisywany rekord
	T getLastRecord();

	//czy kolejny rekord jest dostepny?
	bool isNextRecordAvaible();

	//zapisz rekord
	void writeRecord(T& rec);
	
	//zapisz recordy uwzgledniajac stronicowanie
	void writeRecords(std::vector<T>& records);

	//zmien mode - pisanie/czytanie
	void changeMode(int mode);

	//zresetuj pozycje w pamieci do 0
	void reset();

	//zakoncz wpisywanie - zapisanie strony i znaku konca danych w pamieci sekwencyjnej
	void setEOF();

	//wyczysc plik
	void clear();

	//destruktor
	~PageIO(void);

};

/* Implementacja templateów - ca³oœæ musi byæ w pliku nag³ówkowym... */
#include "PageIO.cpp"