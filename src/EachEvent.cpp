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
	NBJets=0;
	NTauJets=0;
	MET=0;
	dMLL=0;
	HT=0;
	Meff=0;
	PTJLeading=0;
	EtaJLeading=0;
	dPhiJJ=0;
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

void EachEvent::SetData(LepSysInfo ZLepInfo)
{
	Weight=Get_Weight();
	NBJets=Get_NBJets();
	NTauJets=Get_NTauJets();
	NJets=Get_NJetsTot()-NTauJets;//-NBjets;
	MET=Get_MET();
	PTJLeading=Get_PTJLeading();
	EtaJLeading=Get_EtaJLeading();
	PTJoverMET=PTJLeading/MET;
	dPhiJJ=Get_dPhiJJ();
	dMLL=0;
	HT=Get_HT();
	Meff=Get_Meff();
	MT=0;//Get_MT(ZLepInfo);
	MT2=0;
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

double EachEvent::Get_PTJLeading()
{
	return ((Jet*)iteJet->second->At(0))->PT;
}

double EachEvent::Get_EtaJLeading()
{
	return ((Jet*)iteJet->second->At(0))->Eta;
}

double EachEvent::Get_dPhiJJ()
{
	if (iteJet->second->GetEntriesFast()<2)
	{
		return 0;
	}
	else
	{
		TLorentzVector j1,j2;
		j1=((Jet*)iteJet->second->At(0))->P4();
		j2=((Jet*)iteJet->second->At(1))->P4();
		return j1->DeltaPhi(j2);
	}
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

double EachEvent::Get_MT(LepSysInfo ZLepInfo)
{
	double METtemp=((MissingET*)iteMET->second->At(0))->MET;
	double METphitemp=((MissingET*)iteMET->second->At(0))->Phi;

	double LepPT=0.0;
	double LepPhi=0.0;
	// The Following algorithm need to be improved! 
	int NUnpairEplus = ZLepInfo.GetN_UnPair_eplus();
	int NUnpairEminus = ZLepInfo.GetN_UnPair_eminus();
	int NUnpairMuplus = ZLepInfo.GetN_Unpair_muplus();
	int NUnpairMuminus = ZLepInfo.GetN_Unpair_muminus();

	int IndexEtemp=999;
	int IndexMutemp=999;

	if (NUnpairEplus==0&&NUnpairEminus!=0)
	{
		IndexEtemp = ZLepInfo.GetIndex_Unpair_eminus(0);
	}
	else if (NUnpairEplus!=0&&NUnpairEminus==0)
	{
		IndexEtemp = ZLepInfo.GetIndex_Unpair_eplus(0);
	}
	else if (NUnpairEplus!=0&&NUnpairEminus!=0)
	{
		IndexEtemp = ZLepInfo.GetIndex_Unpair_eminus(0)<ZLepInfo.GetIndex_Unpair_eplus(0)?ZLepInfo.GetIndex_Unpair_eminus(0):ZLepInfo.GetIndex_Unpair_eplus(0);
	}

	if (NUnpairMuplus==0&&NUnpairMuminus!=0)
	{
		IndexMutemp = ZLepInfo.GetIndex_Unpair_muminus(0);
	}
	else if (NUnpairMuplus!=0&&NUnpairMuminus==0)
	{
		IndexMutemp = ZLepInfo.GetIndex_Unpair_muplus(0);
	}
	else if (NUnpairMuplus!=0&&NUnpairMuminus!=0)
	{
		IndexMutemp = ZLepInfo.GetIndex_Unpair_muminus(0)<ZLepInfo.GetIndex_Unpair_muplus(0)?ZLepInfo.GetIndex_Unpair_muminus(0):ZLepInfo.GetIndex_Unpair_muplus(0);
	}


	if (IndexEtemp!=999&&IndexMutemp==999)
	{
		LepPT = ((Electron*)iteEle->second->At(IndexEtemp))->PT;
		LepPhi = ((Electron*)iteEle->second->At(IndexEtemp))->Phi;
	}
	else if (IndexEtemp==999&&IndexMutemp!=999)
	{
		LepPT = ((Muon*)iteMuon->second->At(IndexMutemp))->PT;
		LepPhi = ((Muon*)iteMuon->second->At(IndexMutemp))->Phi;
	}
	else if (IndexEtemp!=999&&IndexMutemp!=999)
	{
		double ePT = ((Electron*)iteEle->second->At(IndexEtemp))->PT;
		double muPT = ((Muon*)iteMuon->second->At(IndexMutemp))->PT;
		double ePhi = ((Electron*)iteEle->second->At(IndexEtemp))->Phi;
		double muPhi = ((Muon*)iteMuon->second->At(IndexMutemp))->Phi;
		if (ePT>muPT)
		{
			LepPT = ePT;
			LepPhi = ePhi;
		}
		else
		{
			LepPT = muPT;
			LepPhi = muPhi;
		}
	}
	else
	{
		cout<<"There is something wrong in Lepton System!"<<endl;
		cout<<"No Unpaired Lepton"<<endl;
	}
	// if (NUnpairEplus+NUnpairEminus!=0&&NUnpairMuplus+NUnpairMuminus==0)
	// {
	// 	if (NUnpairEplus==0)
	// 	{
	// 		LepPT=((Electron*)iteEle->second->At(GetIndex_Unpair_eminus(0)))->PT;
	// 		LepPhi=((Electron*)iteEle->second->At(GetIndex_Unpair_eminus(0)))->Phi;
	// 	}
	// 	else if (NUnpairEminus==0)
	// 	{
	// 		LepPT=((Electron*)iteEle->second->At(GetIndex_Unpair_eplus(0)))->PT;
	// 		LepPhi=((Electron*)iteEle->second->At(GetIndex_Unpair_eplus(0)))->Phi;
	// 	}
	// 	else
	// 	{
	// 		int Indextemp = GetIndex_Unpair_eplus(0)<GetIndex_Unpair_eminus(0)?GetIndex_Unpair_eplus(0):GetIndex_Unpair_eminus(0);
	// 		LepPT=((Electron*)iteEle->second->At(Indextemp))->PT;
	// 		LepPhi=((Electron*)iteEle->second->At(Indextemp))->Phi;
	// 	}
	// }
	// else if (NUnpairMuminus+NUnpairMuplus!=0&&NUnpairEplus+NUnpairEminus==0)
	// {
	// 	if (NUnpairMuplus==0)
	// 	{
	// 		LepPT=((Muon*)iteMuon->second->At(GetIndex_Unpair_muminus(0)))->PT;
	// 		LepPhi=((Muon*)iteMuon->second->At(GetIndex_Unpair_muminus(0)))->Phi;
	// 	}
	// 	else if (NUnpairMuminus==0)
	// 	{
	// 		LepPT=((Muon*)iteMuon->second->At(GetIndex_Unpair_muplus(0)))->PT;
	// 		LepPhi=((Muon*)iteMuon->second->At(GetIndex_Unpair_muplus(0)))->Phi;
	// 	}
	// 	else
	// 	{
	// 		int Indextemp = 
	// 	}
	// }

	// int Neles = (iteEle->second->GetEntriesFast());
	// int Nmuons = (iteMuon->second->GetEntriesFast());
	// if(Neles!=0&&Nmuons==0)
	// {
	// 	LepPT=((Electron*)iteEle->second->At(0))->PT;
	// 	LepPhi=((Electron*)iteEle->second->At(0))->Phi;
	// }
	// else if(Nmuons!=0&&Neles==0)
	// {
	// 	LepPT=((Muon*)iteMuon->second->At(0))->PT;
	// 	LepPhi=((Muon*)iteMuon->second->At(0))->Phi;
	// }
	// else
	// {
	// 	cout<<"In EachEvent MT(), there may be some problems!"<<endl;
	// }
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
int EachEvent::Get_NBJets()
{
	int NBJetsTemp=0;
	int NJetsTot=iteJet->second->GetEntriesFast();
	for (int njet = 0; njet < NJetsTot; ++njet)
		{
			NBJetsTemp+=(((Jet*)iteJet->second->At(njet))->BTag & (1<<0));
		}
	return NBJetsTemp;
}

int EachEvent::Get_NTauJets()
{
	int NTauJetsTemp=0;
	int NJetsTot=iteJet->second->GetEntriesFast();
	for (int njet = 0; njet < NJetsTot; ++njet)
	{
		NTauJetsTemp+=(((Jet*)iteJet->second->At(njet))->TauTag & (1<<0));
	}
	return NTauJetsTemp;
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
