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

void BasicCutsOutput(string proName, string proPath,string outdir,string control,string tag="")
{
	TreeReader *reader = new TreeReader("./config/delphes_card.dat");
	TChain *chain = new TChain("Delphes");
	string rootFiles;
	if (control=="all")
	{
		rootFiles = proPath+"/*.root";
	}
	else if (control=="single")
	{
		rootFiles = proPath+"/"+proName+".root";
	}

	chain->Add(rootFiles.c_str());
	reader->SetTreeChain(chain);
	Long64_t allEntries = reader->reader->GetEntries();
	BasicCuts *MyBasic = new BasicCuts();
	MyBasic->GetBranch(reader);
	bool good =1;
	int left=0;
	char output[200];
	if (tag=="")
	{
		sprintf(output,"%s/%s_out_af_basic.root",outdir.c_str(),proName.c_str());
	}
	else
	{
		sprintf(output,"%s/%s_%s_out_af_basic.root",outdir.c_str(),proName.c_str(),tag.c_str());
	}
	
	//string output = proName+"out_af_basic.root";
	TFile *f1 = new TFile(output,"recreate");
	TTree *t1 = new TTree("af_Basic","Event after Basic Cuts");
	EachEvent *event = new EachEvent();
	event->SetSource(reader);

	t1->Branch("event","EachEvent",&event);

	for (int j = 0; j < allEntries; ++j)
	{
	    reader->GetEntry(j);
		good = MyBasic->process();
		if(good) 
		{
			left++;
	    	event->SetData();
	 		t1->Fill();
	 	}
 	}
	f1->Write();
	f1->Close();
	delete event;
	delete MyBasic;
	delete chain;
	delete reader;
}

void background_BasicLoop(char const *filename)
{
	ifstream configfile(filename,ios::in);
	string tag,outdir,proName,proPath,proTag;
	int Nbkg,Npath;
	double cs,kfactor;
	while(configfile)
	{
		configfile>>tag;
		if (tag=="#")
		{
			configfile.ignore(999,'\n');
		}
		else if (tag=="OUTPUT_PATH:")
		{
			configfile>>outdir;
			configfile.ignore(999,'\n');
		}
		else if (tag=="NB")
		{
			configfile>>Nbkg;
			configfile.ignore(999,'\n');
			for (int nb = 0; nb < Nbkg; ++nb)
			{
				configfile>>proName>>Npath;
				for (int np = 0; np < Npath; ++np)
				{
					configfile>>proTag>>cs>>kfactor>>proPath;
					BasicCutsOutput(proName,proPath,outdir,"all",proTag);
				}
				configfile.ignore(999,'\n');
			}
		}
		else
		{
			configfile.ignore(999,'\n');
		}
		char first;
		configfile.get(first);
		configfile.putback(first);
	}
}

void signal_BasicLoop(char const *filename)
{
	ifstream configfile(filename,ios::in);
	string tag,inputdir,outputdir,proName;
	int Nsignal;
	while(configfile)
	{
		configfile>>tag;
		if(tag=="INPUT_PATH:")
		{
			configfile>>inputdir;
			configfile.ignore(999,'\n');
		}
		else if (tag=="OUTPUT_PATH:")
		{
			configfile>>outputdir;
			configfile.ignore(999,'\n');
		}
		else if (tag=="NS")
		{
			configfile>>Nsignal;
			configfile.ignore(999,'\n');
			for (int i = 0; i < Nsignal; ++i)
			{
				configfile>>proName;
				configfile.ignore(999,'\n');
				BasicCutsOutput(proName,inputdir,outputdir,"single");
			}
		}
		else
		{
			configfile.ignore(999,'\n');
		}
		char first;
		configfile.get(first);
		configfile.putback(first);
	}
}

int main(int argc, char const *argv[])
{

//	background_BasicLoop("./config/background_loop");
	signal_BasicLoop("./config/signal_loop");

	return 0;
}


