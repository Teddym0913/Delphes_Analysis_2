#include "TROOT.h"
#include "TreeReader.h"
#include "TChain.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"

using namespace std;

int main(/*int argc, char const *argv[]*/)
{
	//gROOT->SetBatch();
	TreeReader *reader = new TreeReader("./config/delphes_card.dat");
	TChain *chain = new TChain("Delphes");
	chain->Add("./*.root");
	reader->SetTreeChain(chain);
	TreeReader::TBranchArray::iterator itMyBranch;
	for (itMyBranch=reader->MyBranch.begin();itMyBranch!=reader->MyBranch.end();++itMyBranch)
	{
		cout<<itMyBranch->first.first<<"  "<<itMyBranch->first.second<<endl;
		if(itMyBranch->first.second=="Jet")
		{
			reader->reader->ReadEntry(500);
			for (int i = 0; i < itMyBranch->second->GetEntriesFast(); ++i)
			{
				Jet *j = (Jet*)itMyBranch->second->At(i);
				cout<<"Jet PT: "<<j->PT<<endl;
			}
		}
	}

	return 0;
}


