#ifndef BASICCUTS_H
#define BASICCUTS_H 
#include <string>
#include <vector>
#include <fstream>
#include "TreeReader.h"
#include "TClonesArray.h"

using namespace std;

class BasicCuts
{
public:
	BasicCuts(const char *config="./config/Cuts_card.dat");
	~BasicCuts();
	bool process(TreeReader *);

	void GetBranch(TreeReader *);

private:
	vector<double> JetPTmin;
	vector<double> LepPTmin;
	int NLepSign[2];
	double dRJJ;
	double dRJL;
	double dRLL;
	double dPhiJMET[2];

	void BasicJetPTmin(TClonesArray *,double);
	void BasicElePTmin(TClonesArray *,double);
	void BasicMuonPTmin(TClonesArray *,double);
	bool BasicLeadingPT(TClonesArray *,vector<double>);
	//bool BasicLeadingPT(TClonesArray *, TClonesArray *, vector<double>);
	//bool BasicdR(TClonesArray*, double);
	void BasicdREle(TClonesArray*,TClonesArray*,double);
	void BasicdRMuon(TClonesArray*,TClonesArray*,double);
	bool BasicdPhi(TClonesArray*,const TClonesArray*,const double*);
	bool BasicNLepSign(TClonesArray*, TClonesArray*, const int*, vector<double>);

	TreeReader::TBranchArray::iterator iteBranchJet;
	TreeReader::TBranchArray::iterator iteBranchEle;
	TreeReader::TBranchArray::iterator iteBranchMuon;
	TreeReader::TBranchArray::iterator iteBranchMET;
};

#endif