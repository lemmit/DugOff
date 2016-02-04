#pragma once
#include "StdAfx.h"
#include "PageIO.h"

/*
Konstruktor klasy PageIO

SequenceIO - klasa abstrakcyjna, realizuje pisanie/czytanie n bajtow, zmiane pozycje w "nosniku" - niskopoziomowy dostep
size - wielkosc strony
recordDelimiter - okresla jakim znakiem rozdzielane sa rekordy w pliku
EOFsign - znak konca pliku
*/
template <typename T> PageIO<T>::PageIO(SequenceIO *sequenceIO, int size, char recordDelimiter, char EOFsign) :
				_pageSize(size),
				_recordDelimiter(recordDelimiter),
				_EOFsign(EOFsign),
				_sequenceIO(sequenceIO){ 
				recordOffset = 0;
				hasBeenRead = 0;
				_pagedRecordsBuffer=NULL;
				_pageBuffer = NULL;
				createPageBuffer(_pageSize);
				firstRecord = true;
}


/* 
Tworzy bufor strony o zadanej wielkosci (wyrownanej do dlugosci Recordu
Nastepnie tworzy tablice wskaznikow do rekordow, uzupelniajac w niej wartosci 
adresami gdzie zaczynaja sie kolejne rekordy.

Standard C++ nie opisuje czy tablica dwuwymiarowa zajmuje kolejne "wiersze" w pamieci
zatem nie mozna wykonac prostego rzutowania.
*/

template <typename T> bool PageIO<T>::createPageBuffer(unsigned int pageSize){
		//Dlugosc rekordu uwzgledniajac delimiter miedzy nimi
		int recSize = T::size() + 1; 
		//Wyrownanie wielkoci strony
		_pageSize = pageSize - (pageSize % recSize);
		//Inicjalizacja pamieci na strone
		_pageBuffer = new unsigned char[_pageSize+1];

		//Inicjalizacja pamieci na tablice z adresami, gdzie zaczynaja sie kolejne recordy
		unsigned char ** _pageRecords = new unsigned char* [_pageSize / recSize];
		if(_pageBuffer==NULL || _pageRecords == NULL) throw "Error while allocating memory";

		//Przypisanie indeksow
		for(int i=0; i<_pageSize/recSize; i++){
			_pageRecords[i] = _pageBuffer + ( i*recSize );
		}
		//_pageRecordsBuffer po tym jest tablica gdzie [0] to binarnie zapisany na T::size()+1 bajtach pierwszy rekord, [1] drugi... 
		_pagedRecordsBuffer = _pageRecords;
		
		return true;
}

/*
Sprawdza czy nastepny rekord jest dostepny
-> czy nie napotkano znaku konca pliku/tasmy
-> na aktualnej stronie
-> czy da sie wczytac nowa strone i powtorzyc sprawdzenie
*/
template <typename T> bool PageIO<T>::isNextRecordAvaible(){
	//koniec strony?
	if(isNextRecordAvaibleOnPage()) {
		//koniec pliku?
		return !isEOF();
	}
	else{
		readPage();
		//jezeli wczytano strone (>0 bajtow) to mozemy sprobowac czytac rekord jeszcze raz
		if(hasBeenRead <= 0) return false;
		else return isNextRecordAvaible();
	}
}

/*
Wczytuje _pageSize bajtow z pamieci sekwencyjnej - plik/tasma
*/
template <typename T> void PageIO<T>::readPage(){
	if(firstRecord) firstRecord = false;
	if(_sequenceIO!=NULL){
		hasBeenRead = _sequenceIO->read(_pageBuffer, _pageSize);
		//przesuwamy indeks na pierwszy wpis w tablicy
		recordOffset = 0;
		//printf("New page read: %d bytes\n", hasBeenRead);
	}
}

/*
Jezeli dostepny - zwraca kolejny rekord i ustawia sie na kolejny
w innym przypadku zwraca niezainicjowany rekord 
*/
template <typename T> T PageIO<T>::getNextRecord(){
	if( isNextRecordAvaible() ){
		_lastRecord = T(_pagedRecordsBuffer[recordOffset]+1);
		recordOffset++;
		return _lastRecord;
	}else return T();
}

/*
Zwraca ostatni rekord zwrocony z getNextRecord lub zapisany na tasmie/pliku
*/
template <typename T> T PageIO<T>::getLastRecord(){
	return _lastRecord;
}

/*
Zapisuje dany rekord na tasmie
-> zapisuje na stronie pamieci
-> jezeli strona sie skonczyla - zapisuje ja na dysk i zaczyna od poczatko
*/
template <typename T> void PageIO<T>::writeRecord(T& rec){
	//wielkosc rekordu w bajtach
	const int recSize = T::size()+1;
	//
	_lastRecord = rec;
	//czy jest miejsce na zapisanie rekordu?
	if(recordOffset <  _pageSize / recSize){

		//skopiowanie rekordu w odpowiednie miejsce
		((unsigned char*)_pagedRecordsBuffer[recordOffset])[0] = _recordDelimiter;
		//delimiter|record
		rec.copyTo((unsigned char*)(_pagedRecordsBuffer[recordOffset]+1));

		//DEBUG
		//printf("Zapisano wpis na pozycji %d :", recordOffset); rec.println();
		
		recordOffset++;
	}else{
		writePage();
		writeRecord(rec);
	}
}

/*
Zapisz wiele rekordow uzywajac stronicowania
*/
template <typename T> void PageIO<T>::writeRecords(std::vector<T>& records){
	for(std::vector<T>::iterator iter = records.begin(); iter!=records.end(); ++iter){
		writeRecord(*iter);
	}
}

/*
Zapisuje strone za pomoca pamieci sekwencyjnej
Zapisywanie rozpoczyna od ostatniego modyfikowanego pola
*/
template <typename T> void PageIO<T>::writePage(){
	int recSize = T::size()+1;
	if(_sequenceIO!=NULL){
		int written = _sequenceIO->write(_pageBuffer, _pageSize);
		//DEBUG
		/*printf("Zapisano strone pamieci: %d \n", written);
		for(int i=0; i<(_pageSize/recSize); i++){
			if(_pagedRecordsBuffer[i][0]=='#') { 
				printf("EOF\n"); 
				break; 
			}
			T rec = T(_pagedRecordsBuffer[i]);
			rec.println();
		}*/
		recordOffset = 0;
	}
}

/*
Reset pliku/tasmy, powrot na poczatek, otworzenie ponowne
*/
template <typename T> void PageIO<T>::reset(){
	recordOffset = 0;
	hasBeenRead = 0;
	firstRecord = true;
	_lastRecord = T(9999.999f, 9999.999f);
	if(_sequenceIO!=NULL)
		_sequenceIO->reset();
}

/*
Wpisanie znaku konca pliku na stronie i zapisanie strony w pliku/na tasmie
Jezeli strona jest pelna, to ja zapisuje i potem kolejna ze znakiem EOF
*/
template <typename T> void PageIO<T>::setEOF(){
	int recSize = T::size()+1;
	int nrOfRecords = ceil((float)_pageSize/(float)recSize);
	if(recordOffset < nrOfRecords){
		_pagedRecordsBuffer[recordOffset][0] =_EOFsign;
		writePage();
	}else{
		writePage();
		setEOF();
	}
}

/*
Czyszczenie pamieci
*/
template <typename T> PageIO<T>::~PageIO(void){
	delete _sequenceIO;
	delete [] _pageBuffer;
	delete [] _pagedRecordsBuffer;
}

/*
Sprawdza czy napotkano koniec pliku
*/
template <typename T> bool PageIO<T>::isEOF(){
	bool iseof = (_pagedRecordsBuffer[recordOffset][0]==_EOFsign);
	//if(iseof) printf("EOF\n");
	return iseof;
}

/*
Sprawdza czy kolejny rekord jest dostepny na aktualnie wczytanej stronie
*/
template <typename T> bool PageIO<T>::isNextRecordAvaibleOnPage(){
	if(firstRecord) return false;
	int recSize = T::size()+1;
	int nrOfRecordsRead = ceil((float)hasBeenRead/(float)recSize);
	return (recordOffset < nrOfRecordsRead);
}

/* Czysci plik i ustawia wskaznik na poczatek pliku */
template <typename T> void PageIO<T>::clear(){
	reset();
	setEOF();
	reset();
}


