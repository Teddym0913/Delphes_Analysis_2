#ifndef EachEvent_h
#define EachEvent_h

#include "TreeReader.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"

// #include "ExRootAnalysis/ExRootTreeReader.h"
// #include "ExRootAnalysis/ExRootTreeWriter.h"

class EachEvent
{
public:
	EachEvent(TreeReader *reader);
	~EachEvent();


private:
	Jet *jets;
	Electron *eles;
	Muon *muons;
	MissingET *MET;

	GenParticle *GenPars;
    Photon *phos;
    ScalarHT *SHT;

//    Advanced_Data *Advanced_Data
};
#endif