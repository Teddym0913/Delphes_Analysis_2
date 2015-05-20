#include "TROOT.h"
#include "TreeReader.h"
#include "BasicCuts.h"
#include "EachEvent.h"
#include "TChain.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "AdvancedCuts.h"
using namespace std;

int main(/*int argc, char const *argv[]*/)
{
	//gROOT->SetBatch();
	TreeReader *reader = new TreeReader("./config/delphes_card.dat");
	TChain *chain = new TChain("Delphes");
	chain->Add("./*.root");
	reader->SetTreeChain(chain);
	Long64_t allEntries = reader->reader->GetEntries();
	BasicCuts *MyBasic = new BasicCuts();
	MyBasic->GetBranch(reader);
	//TreeReader::TBranchArray::iterator iteBranch = reader->MyBranch.find(make_pair("Jet","Jet"));
	//if(iteBranch==reader->MyBranch.end()) cout<<"Wrong!"<<endl;
	//cout<<iteBranch->first.first<<endl;
	bool good =1;
	int left=0;

	TFile *f1 = new TFile("test.root","recreate");
	TTree *t1 = new TTree("t1","test");
	EachEvent *event = new EachEvent();
	event->SetSource(reader);

	t1->Branch("event","EachEvent",&event);

	for (int j = 0; j < allEntries; ++j)
	{
	    reader->GetEntry(j);
	    //event->SetSource(reader->MyBranch);
		//cout<<j<<endl;
		good = MyBasic->process();
		if(good) 
		{
			left++;
	    	event->SetData();
	 		t1->Fill();
	 	}
 	}
	f1->Write();
	cout<<left<<endl;
	return 0;
}


