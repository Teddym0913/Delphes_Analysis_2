#include "EachEvent.h"

EachEvent::EachEvent()
{
	NJets=0;
	NBjets=0;
	MET=0;
	dMll=0;
	HT=0;
	Meff=0;
	MT=0;
	MT2=0;
}

EachEvent::~EachEvent()
{}

void EachEvent::SetData(int i)
{
	NJets=i;
	NBjets=i;
	MET=i;
	dMll=i;
	HT=i;
	Meff=i;
	MT=i;
	MT2=i;
}