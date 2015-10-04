#ifndef EachEvent_h
#define EachEvent_h

#include "TreeReader.h"
#include "LepSys.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
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

	int Ntotal;
	double Weight;
	int NJets;
	int NBjets;
	double HT;
	double Meff;
	double MT;
	double MET;
	double dMLL;
	double MT2;

private:
	TreeReader::TBranchArray::iterator iteJet; //! Jets iterator
	TreeReader::TBranchArray::iterator iteEle; //! Electron iterator
	TreeReader::TBranchArray::iterator iteMuon; //! Muon iterator
	TreeReader::TBranchArray::iterator iteMET; //! MET iterator
	TreeReader::TBranchArray::iterator iteEvent; //! Event iterator
//	BasicCuts *MyBasicCuts;


public:
	void SetSource(TreeReader *);
	void SetData(LepSysInfo );
	void SetMT2LorentzVector(TLorentzVector&, TLorentzVector&);
//    Advanced_Data *Advanced_Data

	double Get_Weight();
	double Get_MET();
	double Get_HT();
	double Get_Meff();
	double Get_MT(LepSysInfo );
	double Get_dMLL();
	double Get_MT2();
	int Get_NJetsTot();
	int Get_NBjets();

	ClassDef(EachEvent,1);

};
#endif