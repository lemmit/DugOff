#include "StdAfx.h"
#include "PolyphaseSort.h"


template <typename T> PolyphaseSort<T>::PolyphaseSort(std::vector<PageIO<T>*> &tapes, int inputTape) : _tapes(tapes), _inputTape(inputTape)
{

}


template <typename T> PolyphaseSort<T>::~PolyphaseSort(void){

}

/* Kopiuje ostatnio przeczytany record (last) z from do to */
template <typename T> int PolyphaseSort<T>::copyRecord(int from, int to){
	//zapisz rekord na tasmie
	T last = _tapes[from]->getLastRecord();
	_tapes[to]->writeRecord(last);
	/*if(debug){
		printf("[SORT] Kopiuje record z %d do %d: ", from, to);	
		last.println();
	}*/
	//czy kolejny rekord bedzie 
	if(_tapes[from]->isNextRecordAvaible()){
		T rec = _tapes[from]->getNextRecord();
		//1 - rec >= last - kontynuacja serii
		if (rec > last || rec == last) return 1;
		//2 rec < last - koniec serii
		else return 2;
	}else return 0; // 0 - koniec wejscia
}

/* 
Kopiuje serie z tasmy nr from do tasmy nr to
*/
template <typename T> int PolyphaseSort<T>::copySerie(int from, int to){
	/*if(debug){
		printf("[SORT] Kopiuje serie z %d do %d \n", from, to);	
	}*/
	int ans = 0;

	do{
		ans = copyRecord(from, to);
	}while( ans == 1 ) ;

	/*if(debug){
		printf("[SORT] Koniec serii. \n", from, to);	
	}*/
	return ans;
}

/* Numer kolejnej tasmy z n mozliwych wykluczajac jedna - sluzy do zmiany tasmy przy kolejnej serii */
inline int next(int inTape, int act, int nrOfTapes){
	/* 
	dla 3 tasm: 0, 1, 2
	i wybranej "inTape" = 1
	0->2
	1->2 <- przypadek tylko przy starcie (pozniej zapetla sie 0 i 2 na zmiane)
	2->0
	*/
	while((++act%nrOfTapes) == inTape) ;
	return act%nrOfTapes;
}

/* Faza 1sza algorytmu polifazowego - dystrybucja */
template <typename T> void PolyphaseSort<T>::Phase1Polyphase(int inTape){

}

/* Faza 2ga algorytmu polifazowego - ³aczenie */
template <typename T> void PolyphaseSort<T>::Phase2Polyphase(int inTape){

}

/* Faza 1sza algorytmu zlaczania naturalnego - dystrybucja na pozosta³e taœmy z wejsciowej */
template <typename T> int PolyphaseSort<T>::Phase1NaturalMerge(int inTape){
	//przesuniecie tasmy na pierwszy rekord
	if(_tapes[inTape]->isNextRecordAvaible())
		_tapes[inTape]->getNextRecord();
	else return 0;

	int bTape = next(inTape, inTape, _tapes.size());
	int cTape = next(inTape, bTape, _tapes.size());
	int ans = 0;
	int series = 0;
	do{		
		//kopiuj serie
		ans = copySerie(inTape, bTape);
		series++;
		//zakonczenie serii - dane nadal sa
		if(ans != 0) {
			ans = copySerie(inTape, cTape);
			series++;
		}
		//dopoki sa elementy na wejsciu...
	}while(ans != 0);

	return series;
}

/* Faza 2ga algorytmu zlaczania naturalnego - ³aczenie na pocz¹tkow¹ taœme */
template <typename T> int PolyphaseSort<T>::Phase2NaturalMerge(int inTape){
	int bTape = next(inTape, inTape, _tapes.size());
	int cTape = next(inTape, bTape, _tapes.size());

	int nrOfSeries = 0;
	/* Lacz serie */
	//jezeli na obu tasmach sa dane - to kontynuujemy
	//oba elementy powinny istnieæ
	if(_tapes[bTape]->isNextRecordAvaible() && _tapes[cTape]->isNextRecordAvaible()){
		_tapes[bTape]->getNextRecord();
		_tapes[cTape]->getNextRecord();
		int ans = 0;
		bool bTapeEnd = false;
		do{
			T recB = _tapes[bTape]->getLastRecord();
			T recC = _tapes[cTape]->getLastRecord();
			if(recB < recC || recB == recC){
				ans = copyRecord(bTape, inTape); 
				if(ans!=1) {
					ans = copySerie(cTape, inTape);
					nrOfSeries++;
				}
			}
			else {
				ans = copyRecord(cTape, inTape);
				if(ans!=1) {
					ans = copySerie(bTape, inTape);
					nrOfSeries++;
				}
			}
		//dopoki nie zakonczy sie wejscie w ktorejs z tasm
		}while(_tapes[bTape]->isNextRecordAvaible() && _tapes[cTape]->isNextRecordAvaible());
	}

	//czy dostepne na tasmie b? przepisz do konca
	while( copySerie(bTape, inTape) != 0){
		nrOfSeries++;
	}

	//czy dostepne na tasmie c? przepisz do konca
	while( copySerie(cTape, inTape)!=0 ){
		nrOfSeries++;		
	}

	return nrOfSeries;
}

/* Wypisuje w kolumnach zawartosc n tasm */
template <typename T> void PolyphaseSort<T>::showTapes(bool useSeriesInfo){
	bool any_avaible = true;

	for(unsigned int i=0; i<_tapes.size(); i++) printf("  TAPE NR:  %d    ", i);	
	printf("\n");
	int j = 0;
	while(any_avaible){
		any_avaible = false;
		printf("%d.", 1+j++);
		for(unsigned int i=0; i<_tapes.size(); i++){
			if(_tapes[i]->isNextRecordAvaible()){
				any_avaible = true; 
				_tapes[i]->getNextRecord().print(); 
			}
			else printf(" *NOT   AVAIBLE* ");
		}
		printf("\n");
	}
	for(unsigned int i=0; i<_tapes.size(); i++) 
		_tapes[i]->reset();
}

/* Czeka na enter po wyswietleniu zawartosci tasm */
template <typename T> void PolyphaseSort<T>::enter(bool debug){
	if(debug){
		showTapes();
		printf("Nacisnij ENTER by kontynuowac:\n");
		char znak;
		do{ 
			scanf("%c", &znak); 
		}while(znak!='\n');
	}
}

/* Sortowanie za pomoca laczenia naturalnego */
template <typename T> int PolyphaseSort<T>::NaturalMergeSort(bool _debug){
	debug = _debug;
	if( _tapes.size() !=3 ) return -1;

	//rozdzielanie na tasmy
	int inTape = _inputTape;
	int bTape = next(inTape, inTape, _tapes.size());
	int cTape = next(inTape, bTape, _tapes.size());

	/* wyczyszczenie tasm na które kopiujemy */
	_tapes[bTape]->clear();
	_tapes[cTape]->clear();
	/* reset tasmy z której kopiujemy */
	_tapes[inTape]->reset();

	int nrOfSeries = 0;
	enter(true);
	do{
		/* Faza 1 */
		if(debug){
			printf("[SORT] Rozdzielanie - kopiujemy z: %d\n", inTape);
		}
		nrOfSeries = Phase1NaturalMerge(inTape);
		printf("[SORT] Liczba serii po 1: %d\n", nrOfSeries);
		/* czyscimy pierwsza, zapisujemy koniec pliku i restartujemy pozosta³e */
		_tapes[inTape]->clear();
		_tapes[bTape]->setEOF(); _tapes[bTape]->reset();
		_tapes[cTape]->setEOF(); _tapes[cTape]->reset();

		enter(debug);
		/* FAZA 2 */
		if(debug){
			printf("[SORT] Laczenie - kopiujemy z pozostalych tasm na: %d\n", inTape);
		}
		
		nrOfSeries = Phase2NaturalMerge(inTape);
		printf("[SORT] Liczba serii po 2: %d\n", nrOfSeries);
		/* wyczyszczenie tasm na które kopiujemy */
		_tapes[bTape]->clear();
		if(debug) printf("_tapes[bTape]->clear()\n");

		_tapes[cTape]->clear();
		if(debug) printf("_tapes[cTape]->clear()\n");

		/* reset tasmy z której kopiujemy */
		_tapes[inTape]->setEOF();
		if(debug) printf("_tapes[inTape]->setEOF()\n");

		_tapes[inTape]->reset();
		if(debug) printf("_tapes[inTape]->reset()\n");

		enter(debug);
		//czy jest juz jedna seria?
	}while(nrOfSeries>=1);

	printf("[SORT] Posortowane\n: ");
	enter(true);

	return inTape;
}

/* Zmienia indeks tasmy wejsciowej */
template <typename T> void PolyphaseSort<T>::changeInTape(int inTape){
	_inTape = inTape;
}

/* Sortowanie metod¹ sortowania polifazowego - zwraca indeks tasmy */
template <typename T> int PolyphaseSort<T>::PolyphaseSorting(bool _debug){
	debug = _debug;
	if( _tapes.size() !=3 ) return -1;

	//rozdzielanie na tasmy
	int inTape = _inputTape;
	int bTape = next(inTape, inTape, _tapes.size());
	int cTape = next(inTape, bTape, _tapes.size());

	/* wyczyszczenie tasm na które kopiujemy */
	_tapes[bTape]->clear();
	_tapes[cTape]->clear();
	/* reset tasmy z której kopiujemy */
	_tapes[inTape]->reset();

	enter(true);

	/* Rozdysponuj serie na tasmach */

	

	return inTape;
}