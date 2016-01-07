#include "TROOT.h"
#include "TH1F.h"
#include "TreeReader.h"
#include "BasicCuts.h"
#include "EachEvent.h"
#include "TChain.h"
#include "TCanvas.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "AdvancedCuts.h"
#include <sstream>
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
		rootFiles = proPath+"/"+proName+"*.root";
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
		    	event->SetData(MyBasic->ZLepInfo);
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
inline void cycle(int n,int NCuts,double *param, AdvancedCuts *adcuts,TTree* Tbas,int Ntotal,double cs,ofstream &outfile)
{
	if (n==0)
	{
		cout<<"False in AdvancedCuts Cycle!"<<endl;
		return;
	}
   for (param[NCuts-n] = (adcuts->MyAdCuts[NCuts-n].second)[0]; param[NCuts-n] <= (adcuts->MyAdCuts[NCuts-n].second)[1]; param[NCuts-n]+=(adcuts->MyAdCuts[NCuts-n].second)[2])
   {
   		if (n>1)
   		{
   			cycle(n-1,NCuts,param,adcuts,Tbas,Ntotal,cs,outfile);
   		}
   		else
   		{
   			stringstream cuts;
   			string Scuts;
   			double GetCounts=0;
   			cuts<<"Weight*(";
   			for (int i = 0; i < NCuts; ++i)
   			{
   				outfile<<adcuts->MyAdCuts[i].first<<">="<<param[i]<<":";
   				cuts<<adcuts->MyAdCuts[i].first<<">="<<param[i]<<"&&";
   			}
   			cuts<<"NJets<=1&&NBjets==0)";
   			cuts>>Scuts;
   			Tbas->Draw("NBjets>>Count(10,0,10)",Scuts.c_str());
   			gPad->Update();
   			TH1F *htemp = (TH1F*) gPad->GetPrimitive("Count");
   			GetCounts=htemp->Integral();
   			//cout<<GetCounts<<"  "<<Ntotal<<endl;
   			//outfile<<"    "<<GetCounts*cs/Ntotal<<endl;
   			outfile<<"    "<<"Counts_Left  Ntotal  cs: "<<GetCounts<<" "<<Ntotal<<" "<<cs<<endl;
		//	outfile<<"    "<<"Counts_Left  Ntotal  cs: "<<GetCounts<<" "<<Ntotal<<" "<<cs<<endl;
   		}
   }
}

void AdvancedCutsOutput(string proName, string proPath, string outdir, double cs,string tag="NO")
{
	AdvancedCuts *myAd = new AdvancedCuts();
	string fin,fout;
	if (tag=="NO")
	{
		fin = proPath + "/" + proName + "_out_af_basic.root";
		fout = outdir + "/" + proName + "_out_af_Advanced.dat";
	}
	else
	{
		fin = proPath + "/" + proName + "_" + tag + "_out_af_basic.root";
		fout = outdir + "/" + proName + "_" + tag + "_out_af_Advanced.dat";
	}
	TFile *f1 = new TFile(fin.c_str(),"read");
	TTree *t1 = (TTree*) f1->Get("af_Basic");
	
	ofstream adfile(fout.c_str(),ios::out);

	int ncuts = myAd->GetNCuts();

	//string *Branchs = new string[ncuts];
	double *VariableValue = new double[ncuts];
	double *param = new double[ncuts];
	//double Weight;
	EachEvent *event = new EachEvent();
	t1->SetBranchAddress("event",&event);
	t1->GetEntry(0);
	// cout<<event->Ntotal<<endl;
	// for (int i = 0; i < ncuts; ++i)
	// {
	// 	t1->SetBranchAddress((myAd->MyAdCuts[i].first).c_str(),&VariableValue[i]);
	// }
	// double Survive=0;
	// for (int i = 0; i < t1->GetEntries(); ++i)
	// {
	// 	t1->GetEntry(i);
	// 	if (event->NBjets==1&&event->NJets==3&&event->HT>350)
	// 	{
	// 		Survive+=event->Weight;
	// 	}
	// }
	// cout<<Survive<<endl;
	// t1->Draw("HT","Weight*(NBjets==1&&NJets==3&&HT>350)");
	// gPad->Update();
	// TH1F* h1 = (TH1F*) gPad->GetPrimitive("htemp");
	// cout<<h1->Integral()<<endl;
	cycle(ncuts,ncuts,param,myAd,t1,event->Ntotal,cs,adfile);
	adfile.close();
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
		else if (tag=="Af_Basic_PATH:")
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

void background_AdvancedLoop(char const *filename)
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
					AdvancedCutsOutput(proName,proPath,outdir,cs*kfactor,proTag);
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

void signal_AdvancedLoop(char const *filename)
{
	ifstream configfile(filename,ios::in);
	string tag,inputdir,outputdir,proName;
	int Nsignal;
	double cs=1;
	double kfactor=1;
	while(configfile)
	{
		configfile>>tag;
		if(tag=="Af_Basic_PATH:")
		{
			configfile>>inputdir;
			configfile.ignore(999,'\n');
		}
		else if (tag=="Af_Advanced_PATH:")
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
				configfile>>proName>>cs;
				configfile.ignore(999,'\n');
				AdvancedCutsOutput(proName,inputdir,outputdir,cs*kfactor);
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
	background_BasicLoop("./config/background_loop");	
	//signal_BasicLoop("./config/signal_loop");
	//background_AdvancedLoop("./config/background_loop");
	//signal_AdvancedLoop("./config/signal_loop");
	//BasicCutsOutput("BenchmarkPoint_150_650","/home/teddy/Newspace/workingspace/MG5_DATA/Case_III_RHsbottom/Background","/home/teddy/Newspace/workingspace/MG5_DATA/Case_III_RHsbottom/Background","single");

	return 0;
}


