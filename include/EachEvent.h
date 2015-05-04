#ifndef EachEvent_h
#define EachEvent_h

// #include "TreeReader.h"
// #include "modules/Delphes.h"
// #include "classes/DelphesClasses.h"
// #include "classes/DelphesFactory.h"
#include "TROOT.h"
#include "TObject.h"

// #include "ExRootAnalysis/ExRootTreeReader.h"
// #include "ExRootAnalysis/ExRootTreeWriter.h"
using namespace std;

class EachEvent
{
public:
	EachEvent();
	~EachEvent();

	int NJets;
	int NBjets;
	double HT;
	double Meff;
	double MT;
	double MET;
	double dMll;
	double MT2;

	void SetData(int i);
//    Advanced_Data *Advanced_Data
	ClassDef(EachEvent,1);
};
#endif