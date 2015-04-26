#ifndef TreeReader_h
#define TreeReader_h

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include "TROOT.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "ExRootAnalysis/ExRootTreeReader.h"

using namespace std;

class TreeReader
{
public:
	TreeReader(char *configfile);
	~TreeReader();
	ExRootTreeReader *reader;
	bool SetTreeChain(TChain *chain);
    bool GetEntry(Long64_t entry);
    // TClonesArray *branchParticle;
    // TClonesArray *branchElectron;
    // TClonesArray *branchPhoton;
    // TClonesArray *branchMuon;

    // TClonesArray *branchTrack;
    // TClonesArray *branchTower;

    // TClonesArray *branchEFlowTrack;
    // TClonesArray *branchEFlowPhoton;
    // TClonesArray *branchEFlowNeutralHadron;
    // TClonesArray *branchJet;
    // TClonesArray *branchSpecialJet;
    // TClonesArray *branchSHT
    typedef map<pair<string,string>, TClonesArray*> TBranchArray;  //The key is the info of branch (first is branch name, second is branch class), the value is the return value of treereader->UseBranch(xx)
    TBranchArray MyBranch;
	//Here I can decide how many branch I will use and what are they. I can read the branch infomation from the delphes card or from some configure files
private:
	vector<pair<string,string> > BranchInfo; // First is the name of branch, the second is the classname of the same branch

};
#endif