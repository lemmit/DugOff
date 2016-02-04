#pragma once
#include <string>
#include <cstdio>
#include "SequenceIO.h"

/*
Klasa specjalizujaca klase abstrakcyjna SequenceIO
Pozwala na sekwencyjny dostep do pliku - otwieranie, pisanie/czytanie n bajtow, ustawianie pozycji
*/

typedef FILE* FileDescriptor;
class SequenceIOFile : public SequenceIO
{
private:
	//Aktualnie otwarty plik
	FileDescriptor _file; 
	//Sciezka do aktualnie otwartego pliku
	std::string _path;

	//Pozycja w pliku
	int _pos;
public:
	//Sciezka pliku do otwarcia
	SequenceIOFile(std::string path);

	~SequenceIOFile(void);

	//Zmienia pozycje w pliku
	int seek(int pos);

	//Czytanie n bajtow w pliku
	int read(unsigned char* dst, int size);

	//Pisanie n bajtow w pliku
	int write(unsigned char* src, int size);

	//reset -> seek do 0
	int reset();

};

