#include "stdafx.h"
#include <vector>
#include <iostream>
#include <ctime>
#include "SequenceIOFile.h"
#include "SectorRecord.h"
#include "PageIO.h"
#include "PolyphaseSort.h"


/* Generowanie testow */
void fillInputFileWithRandomData(int howMany, const char* inputFile){
	srand(time(NULL));
	FILE* f = fopen(inputFile, "wb");
	fclose(fopen("b.dat", "w")); //czyszczenie
	fclose(fopen("c.dat", "w")); //czyszczenie

	const int size = 2*sizeof(double)+1;
	double x;
	double y;
	unsigned char cbuf[size];
	double *dbuf = (double*)(cbuf+1);
	for(int i=0; i<howMany; i++){
		x = rand() / 10000.f;
		y = rand() / 10000.f;
		cbuf[0] = '\n';
		dbuf[0] = x;
		dbuf[1] = y;
		fwrite(cbuf, 1, size, f);
	}
	cbuf[0] = 0;
	fwrite(cbuf, 1, size, f);

	fclose(f);
}

/* Tworzenie tasm wej-wyj */
void createTapes(std::vector<PageIO<SectorRecord>*> &tapes, const char* inputFile, int page_size){

	SequenceIOFile* seq = new SequenceIOFile(inputFile);
	if(seq==NULL) throw "inputFile nie udalo sie utworzyc";
	tapes.push_back(new PageIO<SectorRecord>(seq, page_size , '\n', 0));

	seq = new SequenceIOFile("b.dat");
	if(seq==NULL) throw "b.dat nie udalo sie utworzyc";
	tapes.push_back(new PageIO<SectorRecord>(seq, page_size, '\n', 0));

	seq = new SequenceIOFile("c.dat");
	if(seq==NULL) throw "c.dat nie udalo sie utworzyc";
	tapes.push_back(new PageIO<SectorRecord>(seq, page_size , '\n', 0));
}

using namespace std;

int main(int argc, char* argv[])
{
	int rec_size = SectorRecord::size() + 1; //wielkosc rekordu powiekszona o delimiter!
	int page_size = rec_size * 30; // wielkosc pamieci
	int debug = 0;
	string inputFile;
	bool inputFileSet = false;
	
	/* Czy podano dodatkowe parametry wywolania? */
	if(argc>2){
		cout << "UHU!\n";
		if(strcmp(argv[1], "/f")){
			FILE* file = fopen(argv[2], "r+b");
			if(file!=0){
				fclose(file);
				inputFile = argv[2];
				inputFileSet = true;
			}else printf("Plik nie istnieje.");
		}
	}else{
		inputFile = "input.dat";
	}

	/* Deklaracja zmiennych uzywanych w switchach */
	vector<PageIO<SectorRecord>*> tapes;
	SectorRecord sec;
	vector<SectorRecord> secs;
	double r;
	double alfa;
	PageIO<SectorRecord>* inTape;
	string str;
	char option;
	FILE* canOpen;
	bool fileNameChanged;
	int out;
	bool exit = false; //czy zakonczyc dzialanie programu
	do{
		cout << "\n1. Ustaw plik wejsciowy.\n"<<
			"2. Zmien poziom debugowania.\n"<<
			"3. Zmien wielkosc strony.\n"<<
			"4. Wprowadz dane.\n"<<
			"5. Sortuj metoda laczenia naturalnego.\n"<<
			"6. Wypelnij plik wejsciowy losowymi danymi.\n"<<
			"7. Wyswietl tasme poczatkowa.\n" <<
			"8. Wyjscie\n" ;
		if(inputFileSet) cout << "Aktualnie wczytany plik wejsciowy: " << inputFile << "\n";
		cout << "Aktualnie ustawiony poziom debugowania: " << debug << "\n";
		cout << "Aktualnie ustawiony rozmiar strony pamieci: " << page_size/SectorRecord::size() << " rekordow, tj. " << page_size << " bajtow\n"; 

		option = 0;
		cin >> option;
		option -= '0';
		switch(option){
		case 1: 
			/* Zmiana pliku wejsciowego */
			cout << "Wprowadz sciezke do pliku:\n";
			cin >> inputFile;
			canOpen = fopen(inputFile.c_str(), "r+b");
			if(canOpen!=0){
				fclose(canOpen);
				inputFileSet = true;
				cout << "Plik ustawiony\n";
			}else {
				cout << "Nie mozna otworzyc pliku\nCzy utworzyc plik(T)?\n";
				option = 0;
				cin >> option;
				option = tolower(option);
				if(option=='t'){
					canOpen = fopen(inputFile.c_str(), "wb");
					if(canOpen!=0){
						fclose(canOpen);
						inputFileSet = true;
						cout << "Utworzono plik\n";
					}else cout << "BLAD!\n";
				}
			}
			break;
		case 2:
			/* Zmiana poziomu debugowania */
			cout << "Zaleznie od poziomu debugowania, program wypisuje wiecej lub mniej dodatkowych informacji:\n"<<
				"1. Wylaczony tryb debugowania - wyswietlaja sie wartosci tasmy przed i po sortowaniu.\n"<<
				"2. Wlaczony tryb debugowania - wyswietlaja sie wartosci pomiedzy etapami sortowania.\n";
			do{ 
				cout << "Podaj poziom: ";
				cin >> debug;
			}while(debug<1 || debug > 2);
			break;
		case 3:
			/* Zmiana wielkosci strony pamieci */
			cout << "\nPodawana liczba n jest iloscia rekordow na stronie pamieci.\nWielkosc zajetej pamieci bedzie wynosic n*"<< SectorRecord::size() << " bajtow\nPodaj wielkosc nowej strony pamieci:\n";
			cin >> page_size;
			if(page_size <= 0) {
				cout << "Bledna wielkosc strony!\n";
				page_size = 10*SectorRecord::size();
			}
			break;
		case 4:
			/* Wprowadzanie danych na tasme wejsciowa */
			do{
				fileNameChanged = false;

				if(inputFileSet) cout << "Dzialanie zapisze dane do aktualnie wybranego pliku wejsciowego!\n \
										Czy kontynuowac(T) czy wprowadzic nazwe nowego pliku(N) czy anulowac (C)?\n";
				option = 0;
				do {
					cin >> option;
					option = tolower(option);
				}while(!(option == 't' || option == 'n' || option == 'c'));

				switch(option){
				case 't':
					do{
						cout << "Wprowadz dane wycinka kola:\nr: ";
						cin >> r;
						cout << "alfa: ";
						cin >> alfa;
						sec = SectorRecord(r,alfa);
						secs.push_back(sec);
						cout << "Czy wprowadzac dane dalej (T) ?\n";
						cin >> option;
						option=tolower(option);
					}while(option=='t');

					//Zapis wektora nowo wprowadzonych danych
					inTape = new PageIO<SectorRecord>(new SequenceIOFile(inputFile.c_str()), page_size , 1, 0);
					inTape->writeRecords(secs);
					inTape->setEOF();
					inTape->reset();
					delete inTape;

					break;
				case 'n':
					cout << "Wprowadz sciezke do pliku:\n";
					cin >> inputFile;
					canOpen = fopen(inputFile.c_str(), "wb");
					if(canOpen!=0){
						fclose(canOpen);
						inputFileSet = true;
						cout << "Utworzono plik\n";
						fileNameChanged = true;
					}else cout << "BLAD!\n";
					break;
				case 'c':
					break;
				}
			}while(fileNameChanged);

			break;
		case 5:/* Sortowanie poprzez laczenie naturalne */
			try{
				createTapes(tapes, inputFile.c_str(), page_size);
				PolyphaseSort<SectorRecord> sort(tapes, 0);
				out = sort.NaturalMergeSort(debug-1);
				cout << "Odp na tasmie: " << out << "\n";
				tapes.clear();
			}
			catch(char* e){
				cout << e << "\n";
			}
			break;
		case 6:
			//Wypelniamy plik wejsciowy przypadkowymi rekordami
			cout << "Ile wygenerowac rekordow?\n";
			cin >> out;
			if(inputFileSet){
				fillInputFileWithRandomData(out, inputFile.c_str());
			}else cout << "Ustaw najpierw plik wejsciowy\n";
			break;
		case 7:
			if(inputFileSet){
				cout <<"Tasma wejsciowa:\n";
				SequenceIOFile* seq = new SequenceIOFile(inputFile);
				PageIO<SectorRecord> *inTape = new PageIO<SectorRecord>(seq, page_size , 1, 0);
				out=1;
				while(inTape->isNextRecordAvaible()){
					cout << out++ << ". ";
					inTape->getNextRecord().println();
				}
				inTape->reset();
				delete inTape;
			}else cout << "Plik nie zaladowany\n";

			break;
		case 8:
			exit = true;
			break;
		default:
			//nic nie robi - kolejny obrot
			break;
		}
		cin.clear();
	}while(!exit);

	return 0;
}