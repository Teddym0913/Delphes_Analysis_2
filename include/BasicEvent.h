#ifndef BasicEvent_h
#define BasicEvent_h 
#include "EachEvent.h"
#include "TreeReader.h"

class BasicEvent
{
public:
	BasicEvent(char *config,TreeReader *treereader);
	~BasicEvent();


private:
	TFile *FBasic;
	TTree *TBasic = new TTree("af_Basic","Events After Basic Cuts!");
	EachEvent *allEvents;
	
};

#endif