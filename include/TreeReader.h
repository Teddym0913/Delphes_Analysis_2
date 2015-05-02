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
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "ExRootAnalysis/ExRootTreeReader.h"

using namespace std;

class TreeReader
{
public:
	TreeReader(const char *configfile);
	~TreeReader();
	bool SetTreeChain(TChain *chain);
    bool GetEntry(Long64_t entry);
    bool InitCuts(char *CutsConfig);

    ExRootTreeReader *reader;
    typedef map<pair<string,string>, TClonesArray*> TBranchArray;  //The key is the info of branch (first is branch name, second is branch class), the value is the return value of treereader->UseBranch(xx)
    TBranchArray MyBranch;
    //TBranchArray::iterator iteJetBranch;
    // vector<Jet*> jets;
    // vector<Electron*> ele;
    
	//Here I can decide how many branch I will use and what are they. I can read the branch infomation from the delphes card or from some configure files
private:
	vector<pair<string,string> > BranchInfo; // First is the name of branch, the second is the classname of the same branch
    // TFile *fBasicOut;
    // TTree *tBasicOut = new TTree("af_Basic","Resutls after Basic Cuts!");
    
};
#endif