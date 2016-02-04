#pragma once

/*
Klasa abstrakcyjna okreslajaca dostep do pamieci sekwencyjnej
pozwala na pisanie/czytanie n bajtow oraz ustawienie pozycji w pamieci w dane miejsce
Uzywana przez PageIO do niskopoziomowego pisania/czytania
*/
class SequenceIO
{
public:
	virtual int seek(int pos) = 0;
	virtual int read(unsigned char* dst, int size) = 0;
	virtual int write(unsigned char* src, int size) =0;
	virtual int reset() = 0;
};

