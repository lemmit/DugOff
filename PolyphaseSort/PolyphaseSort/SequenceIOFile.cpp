#include "StdAfx.h"
#include "SequenceIOFile.h"

/*
Klasa specjalizujaca klase abstrakcyjna SequenceIO
Pozwala na sekwencyjny dostep do pliku - otwieranie, pisanie/czytanie n bajtow, ustawianie pozycji
*/

/* Otwarcie pliku o zadanej sciezce */
SequenceIOFile::SequenceIOFile(std::string path){
	_path = path;
	_pos = 0;
	_file = fopen(_path.c_str(), "r+b"); 
	/* Otworz plik, jezeli nie istnieje, to stworz go i otworz w trybie czytania/modyfikacji */
	if(_file == NULL){
		_file = fopen(_path.c_str(), "w"); 
		if(_file == NULL) throw "Problem z tworzeniem pliku";
		else fclose(_file);
	}else fclose(_file);
}

SequenceIOFile::~SequenceIOFile(void){
	if(_file!=NULL) fclose(_file); //to sie nigdy nie zdarzy
}


/* Przesuniecie pozycji w pliku na dana */
int SequenceIOFile::seek(int pos){
	_pos = pos;
	return 0;
}

/* Zczytanie n bajtow z pliku */
int SequenceIOFile::read(unsigned char* dst, int size){
	//otwieram plik
	_file = fopen(_path.c_str(), "r+b");
	if(_file!=NULL){
		//ustawiam pozycje na ostatnia czytana
		fseek(_file, _pos, SEEK_SET);
		//czytam
		int ret = fread(dst, sizeof(char), size, _file);
		//pobieram pozycje
		_pos = ftell(_file);
		//zamykam plik
		fclose(_file);
		return ret;
	}else return -1;
}

/* Zapisanie n bajtow do pliku */
int SequenceIOFile::write(unsigned char* src, int size){
	_file = fopen(_path.c_str(), "r+b");
	if(_file!=NULL){
		//ustawiam pozycje
		fseek(_file, _pos, SEEK_SET);
		//pisze
		int written = fwrite(src, sizeof(char), size, _file);
		//pobieram pozycje
		_pos = ftell(_file);
		//zamykam plik
		fclose(_file);
		return written;
	}else return -1;
}

/* Ustawienie pozycji w pliku na 0*/
int SequenceIOFile::reset(){
	_pos = 0;
	return 0;
}