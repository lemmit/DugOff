#pragma once
#include <vector>
#include "PageIO.h"
/*
Klasa sortujaca dane na 3 tasmach
Do pisania i czytania rekord�w uzywa PageIO
*/
template <typename T>
class PolyphaseSort
{
private:
	//kopiuje record z from do to
	int copyRecord(int from, int to);

	//kopiuje serie z from do to
	int copySerie(int from, int to);

	/* Faza 1sza algorytmu polifazowego - dystrybucja */
	void Phase1Polyphase(int inTape);

	/* Faza 2ga algorytmu polifazowego - �aczenie */
	void Phase2Polyphase(int inTape);

	/* Faza 1sza algorytmu polifazowego - dystrybucja na pozosta�e ta�my z wejsciowej */
	int Phase1NaturalMerge(int inTape);

	/* Faza 2ga algorytmu polifazowego - �aczenie na pocz�tkow� ta�me */
	int Phase2NaturalMerge(int inTape);

	//ktora tasma jest tasma wejsciowa?
	int _inputTape;

	//tasmy
	std::vector<PageIO<T> * > _tapes;

	//opis serii na poszczeg�lnych ta�mach
	std::vector<int> _series;

	//czy debug?
	bool debug;

	//wypisuje tasmy
	void showTapes(bool useSeriesInfo=false);

	//debug - wypisywanie tasm i oczekiwanie na enter
	void enter(bool debug);
	
public:
	PolyphaseSort(std::vector<PageIO< T > *> &tapes, int inputTape);

	//zmiana indeksu tasmy wejsciowej
	void changeInTape(int inTape);
	//sortuj tasmy - zwraca indeks tasmy z posortowana zawartoscia
	int NaturalMergeSort(bool debug=false);

	//sortuj metod� sortowania polifazowego - zwraca indeks tasmy
	int PolyphaseSorting(bool debug = false);
	~PolyphaseSort(void);
};

#include "PolyphaseSort.cpp"
