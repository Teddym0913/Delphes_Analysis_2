#include "EachEvent.h"
#include "Variables.h"
#include "Mt2/Basic_Mt2_332_Calculator.h"
#include "Mt2/ChengHanBisect_Mt2_332_Calculator.h"

#ifndef MZ
#define MZ 91.188
#endif

EachEvent::EachEvent()
{
	Ntotal=0;
	Weight=1;
	NJets=0;
	NBjets=0;
	MET=0;
	dMLL=0;
	HT=0;
	Meff=0;
	MT=0;
	MT2=0;
}

EachEvent::~EachEvent()
{}

void EachEvent::SetSource(TreeReader* reader)
{
	iteJet = reader->MyBranch.find(make_pair("Jet","Jet"));
	iteEle = reader->MyBranch.find(make_pair("Electron","Electron"));
	iteMuon = reader->MyBranch.find(make_pair("Muon","Muon"));
	iteMET = reader->MyBranch.find(make_pair("MissingET","MissingET"));
	iteEvent = reader->MyBranch.find(make_pair("Event","LHEFEvent"));
	Ntotal = reader->reader->GetEntries();
}

void EachEvent::SetData()
{
	Weight=Get_Weight();
	NBjets=Get_NBjets();
	NJets=Get_NJetsTot();//-NBjets;
	MET=Get_MET();
	//dMLL=0;
	HT=Get_HT();
	//Meff=Get_Meff();
	//MT=Get_MT();
	//MT2=0;
}

double EachEvent::Get_Weight()
{
	double weight = ((LHEFEvent *)iteEvent->second->At(0))->Weight;
	return weight==0?1:weight;
}

double EachEvent::Get_MET()
{
	return ((MissingET *)iteMET->second->At(0))->MET;
}

double EachEvent::Get_HT()
{
	//if(iteJet==NULL) return 0.0;
	double HTtemp=0;
	int njets = iteJet->second->GetEntriesFast();
	for (int i = 0; i < njets; ++i)
	{
		HTtemp+=((Jet*)iteJet->second->At(i))->PT;
	}
	return HTtemp;
} 

double EachEvent::Get_Meff()
{
	double Mefftemp=0.0;
	//if (iteJet!=NULL)
	//{
		int Njets = iteJet->second->GetEntriesFast();
		for (int njet = 0; njet < Njets; ++njet)
		{
			Mefftemp+=((Jet*)iteJet->second->At(njet))->PT;
		}
	//}
	//if (iteEle!=NULL)
	//{
		int NEles = iteEle->second->GetEntriesFast();
		for (int nele = 0; nele < NEles; ++nele)
		{
			Mefftemp+=((Electron*)iteEle->second->At(nele))->PT;
		}
	//}
	//if (iteMuon!=NULL)
	//{
		int NMuons = iteMuon->second->GetEntriesFast();
		for (int nmuon = 0; nmuon < NMuons; ++nmuon)
		{
			Mefftemp+=((Muon*)iteMuon->second->At(nmuon))->PT;
		}
	//}
	//if (iteMET!=NULL)
	//{
		Mefftemp+=((MissingET*)iteMET->second->At(0))->MET;
	//}
	return Mefftemp;
}

double EachEvent::Get_MT()
{
	double METtemp=((MissingET*)iteMET->second->At(0))->MET;
	double METphitemp=((MissingET*)iteMET->second->At(0))->Phi;

	double LepPT=0.0;
	double LepPhi=0.0;

	int Neles = (iteEle->second->GetEntriesFast());
	int Nmuons = (iteMuon->second->GetEntriesFast());
	if(Neles!=0&&Nmuons==0)
	{
		LepPT=((Electron*)iteEle->second->At(0))->PT;
		LepPhi=((Electron*)iteEle->second->At(0))->Phi;
	}
	else if(Nmuons!=0&&Neles==0)
	{
		LepPT=((Muon*)iteMuon->second->At(0))->PT;
		LepPhi=((Muon*)iteMuon->second->At(0))->Phi;
	}
	else
	{
		cout<<"In EachEvent MT(), there may be some problems!"<<endl;
	}
	double MT;
	MT=sqrt(2*LepPT*METtemp*(1-cos(LepPhi-METphitemp)));
	return MT;
}

double EachEvent::Get_dMLL()
{
	TLorentzVector Pl1;
	TLorentzVector Pl2;

	int Neles = (iteEle->second->GetEntriesFast());
	int Nmuons = (iteMuon->second->GetEntriesFast());
	if(Nmuons==0&&Neles!=0)
	{
		if(iteEle->second->GetEntriesFast()<2) return 0.0;
		Pl1 = ((Electron*)iteEle->second->At(0))->P4();
		Pl2 = ((Electron*)iteEle->second->At(1))->P4();
	}
	else if(Neles==0&&Nmuons!=0)
	{
		if(iteMuon->second->GetEntriesFast()<2) return 0.0;
		Pl1 = ((Muon*)iteMuon->second->At(0))->P4();
		Pl2 = ((Muon*)iteMuon->second->At(1))->P4();
	}
	else if(Nmuons!=0&&Neles!=0)
	{
		Pl1 = ((Electron*)iteEle->second->At(0))->P4();
		Pl2 = ((Muon*)iteMuon->second->At(0))->P4();
	}
	else
	{
		cout<<"In EachEvent dMLL(), there may be some problems!"<<endl;
	}
	return fabs((Pl1+Pl2).M()-MZ);
}

int EachEvent::Get_NJetsTot()
{
	return iteJet->second->GetEntriesFast();
}

int EachEvent::Get_NBjets()
{
	int NBjetsTemp=0;
	int NJetsTot=iteJet->second->GetEntriesFast();
	for (int njet = 0; njet < NJetsTot; ++njet)
		{
			NBjetsTemp+=(((Jet*)iteJet->second->At(njet))->BTag & (1<<0));
		}
	return NBjetsTemp;
}

double EachEvent::Get_MT2()
{
	TLorentzVector PSA;
	TLorentzVector PSB;
	SetMT2LorentzVector(PSA,PSB);
	double METtemp=((MissingET*)iteMET->second->At(0))->MET;
	double METphitemp=((MissingET*)iteMET->second->At(0))->Phi;

	Mt2::Basic_Mt2_332_Calculator mt2Calculator;
  	const double massOfSystemA = PSA.M();
  	const double pxOfSystemA = PSA.Px();
  	const double pyOfSystemA = PSA.Py();
  
 	const double massOfSystemB = PSB.M();
  	const double pxOfSystemB = PSB.Px();
  	const double pyOfSystemB = PSB.Py();
  
  	const double pxMiss = METtemp*cos(METphitemp);
  	const double pyMiss = METtemp*sin(METphitemp);

  	const double invis_mass = 0.0;

  	Mt2::LorentzTransverseVector  vis_A(Mt2::TwoVector(pxOfSystemA, pyOfSystemA), massOfSystemA);
  	Mt2::LorentzTransverseVector  vis_B(Mt2::TwoVector(pxOfSystemB, pyOfSystemB), massOfSystemB);
  	Mt2::TwoVector                pT_Miss(pxMiss, pyMiss);
   
    const double mt2 = mt2Calculator.mt2_332(vis_A, vis_B, pT_Miss, invis_mass);
    return mt2;
}

void EachEvent::SetMT2LorentzVector(TLorentzVector &TLVA, TLorentzVector &TLVB)
{
	int Neles = (iteEle->second->GetEntriesFast());
	int Nmuons = (iteMuon->second->GetEntriesFast());
	if(Nmuons==0&&Neles!=0)
	{
		TLVA=((Electron*)iteEle->second->At(0))->P4();
		if(iteEle->second->GetEntriesFast()<2)
		{
			TLVB.SetPtEtaPhiM(0,0,0,0);
		}
		else
		{
			TLVB=((Electron*)iteEle->second->At(1))->P4();
		}
	}
	else if(Neles==0&&Nmuons!=0)
	{
		TLVA=((Muon*)iteMuon->second->At(0))->P4();
		if(iteMuon->second->GetEntriesFast()<2)
		{
			TLVB.SetPtEtaPhiM(0,0,0,0);
		}
		else
		{
			TLVB=((Muon*)iteMuon->second->At(1))->P4();
		}
	}
	else
	{
		TLVA.SetPtEtaPhiM(0,0,0,0);
		TLVB.SetPtEtaPhiM(0,0,0,0);
	}

}
