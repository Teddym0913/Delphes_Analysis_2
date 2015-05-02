#ifndef EachEvent_h
#define EachEvent_h

#include "TreeReader.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"


// #include "ExRootAnalysis/ExRootTreeReader.h"
// #include "ExRootAnalysis/ExRootTreeWriter.h"
using namespace std;

class EachEvent
{
public:
	EachEvent(TreeReader *reader);
	~EachEvent();


private:
	vector<Jet*> jets;
	vector<Electron*> eles;
	vector<Muon*> muons;
	vector<MissingET*> MET;

	vector<GenParticle*> GenPars;
    vector<Photon*> phos;
    vector<ScalarHT*> SHT;

//    Advanced_Data *Advanced_Data
};
#endif