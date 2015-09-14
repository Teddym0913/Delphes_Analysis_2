#ifndef BASICCUTS_H
#define BASICCUTS_H 
#include <string>
#include <vector>
#include <fstream>
#include "TreeReader.h"
#include "TClonesArray.h"

using namespace std;
class ZCandidatesInfo
{
public:
	ZCandidatesInfo();
	~ZCandidatesInfo();

	void Reset();
	void Insert(int,int,int);
	int GetNZ();
	int GetZLepInfo1(int);
	int GetZLepInfo2(int);

private:
	int NZ;
	vector<int> LepFlavor;
	vector<int> Lep1;
	vector<int> Lep2;
};

class BasicCuts
{
public:
	BasicCuts(const char *config="./config/Cuts_card.dat");
	~BasicCuts();
	bool process();
	void GetBranch(TreeReader *);
	ZCandidatesInfo ZLepInfo;

private:
	vector<double> JetPTmin;
	vector<double> RJetPTmin;
	vector<double> BJetPTmin;
	vector<double> LepPTmin;
	int NLepNZ[2];
	double EtamaxJet;
	double EtamaxLep;
	double dRJJ;
	double dRJL;
	double dRLL;
	double dPhiJMET[2];

	void BasicJetPTminEtamax(TClonesArray *,double,double);
	void BasicElePTminEtamax(TClonesArray *,double,double);
	void BasicMuonPTminEtamax(TClonesArray *,double,double);
	bool BasicLeadingPT(TClonesArray *,vector<double>);
	bool BasicLeadingPT(TClonesArray *,vector<double>,vector<double>); // Select the leading Regular Jet PT and leading B-Jet PT also require the minimum number of Regular Jet and B-Jet
	//bool BasicLeadingPT(TClonesArray *, TClonesArray *, vector<double>);
	//bool BasicdR(TClonesArray*, double);
	void BasicdREle(TClonesArray*,TClonesArray*,double);
	void BasicdRMuon(TClonesArray*,TClonesArray*,double);
	bool BasicdPhi(TClonesArray*,const TClonesArray*,const double*);
	bool BasicNLepNZ(TClonesArray*, TClonesArray*, const int*, vector<double>);

	TreeReader::TBranchArray::iterator iteBranchJet;
	TreeReader::TBranchArray::iterator iteBranchEle;
	TreeReader::TBranchArray::iterator iteBranchMuon;
	TreeReader::TBranchArray::iterator iteBranchMET;
};

#endif