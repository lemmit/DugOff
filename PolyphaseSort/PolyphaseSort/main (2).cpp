#include "stdafx.h"
#include "PageIO.h"
#include "SequenceIOFile.h"
#include <vector>

void print(Record &rec){
	printf("%f, %f\n",rec.getAlfa(), rec.getR());
}

void fillInputFileWithDummyData(){
	FILE* f = fopen("input.dat", "wb");
	const int size = 1+8+8;
	double buf[3];
	for(int i=0; i<35; i++){
		double x = i;
		double y = i+0.1f;
		buf[0] = x;
		buf[1] = y;
		unsigned char *cbuf = (unsigned char*)buf;
		cbuf[2*8] = '\n';

		//debug
		/*
		printf("Gen: ");
		for(int j=0; j<17; j++) printf("%d ", (int)cbuf[j]);
		printf("\n");
		*/

		fwrite(cbuf, 1, size, f);
	}
	fwrite("#", 1, 1, f);
	fclose(f);
}

void createTapes(std::vector<PageIO*> &tapes, int rec_size){
	SequenceIOFile* seq = new SequenceIOFile("input.dat");
	tapes.push_back(new PageIO(seq, rec_size /*page size*/, '\n')); // tasma wejsciowa
	tapes[0]->changeMode(PageIO::MODE::READ);

	seq = new SequenceIOFile("b.dat");
	tapes.push_back(new PageIO(seq, rec_size /*page size*/, '\n')); // tasma wejsciowa
	tapes[1]->changeMode(PageIO::MODE::WRITE);

	seq = new SequenceIOFile("c.dat");
	tapes.push_back(new PageIO(seq, rec_size /*page size*/, '\n')); // tasma wejsciowa
	tapes[2]->changeMode(PageIO::MODE::WRITE);
}

void destroyTapes(std::vector<PageIO*> tapes){
	for(std::vector<PageIO*>::iterator iter = tapes.begin(); iter!=tapes.end(); ++iter){
		delete *iter;
	}
	tapes.clear();
}

int main(int argc, char* argv[])
{
	//fillInputFileWithDummyData();
	int rec_size = sizeof(Record) + 1; //delimiter!
	int page_size = rec_size * 15;
	std::vector<PageIO*> tapes;
	try{
		createTapes(tapes, page_size);
		for(int i =0; i<3; i++){
		//1st record
		Record rec;
		if(tapes[i]->isNextRecordAvaible()){
				rec = tapes[i]->getNextRecord();
				printf("1szy record\n");
				print(rec);
		}

		//seria?
		while(tapes[i]->isNextRecordAvaible()){
			Record a = tapes[i]->getNextRecord();
			if(rec<a){
				printf("Nadal seria!\n");
				print(a);
			}
			rec = a;
			if(i==0)tapes[1]->writeRecord(a);
		}
		if(i==0){
			tapes[1]->setEOF();
			tapes[1]->flush();
			tapes[1]->changeMode(PageIO::MODE::READ);
		}
		}
	}
	catch(char* e){
		printf("%s\n" , e);
	}
	destroyTapes(tapes);
	return 0;
}

