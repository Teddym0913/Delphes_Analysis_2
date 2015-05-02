#include "BasicCuts.h"
#include "TLorentzVector.h"
#include <algorithm>

BasicCuts::BasicCuts(const char *configfile)
{
	ifstream config(configfile,ios::in);
	string tag,tag_inner,NameCate;
	int Num;
	double PTmin;
	while(config)
	{
		config>>tag;
		if(tag=="Basic")
		{
			config.ignore(999,'\n');
			while(config)
			{
				config>>tag_inner;
				if(tag_inner=="{")
				{
					config.ignore(999,'\n');
				}
				else if(tag_inner=="}")
				{
					break;
				}
				else
				{
					if(tag_inner=="PTmin")
					{
						config>>NameCate>>Num;
						if(NameCate=="Jets")
						{
							for (int i = 0; i < Num; ++i)
							{
								config>>PTmin;
								JetPTmin.push_back(PTmin);
							}
						}
						else if(NameCate=="Leps")
						{
							for (int i = 0; i < Num; ++i)
							{
								config>>PTmin;
								LepPTmin.push_back(PTmin);
							}
						}
					}
					else if(tag_inner=="dRJJ")
					{
						config>>dRJJ;
					}
					else if(tag_inner=="dRJL")
					{
						config>>dRJL;
					}
					else if(tag_inner=="dRLL")
					{
						config>>dRLL;
					}
					else if(tag_inner=="dPhiJMET")
					{
						config>>dPhiJMET[0]>>dPhiJMET[1];
					}
					else if(tag_inner=="NLep&&Sign")
					{
						config>>NLepSign[0]>>NLepSign[1];
					}
					config.ignore(999,'\n');
				}
			}
		}
		else
		{
			config.ignore(999,'\n');
		}
	}
}

BasicCuts::~BasicCuts()
{
	
}

bool BasicCuts::process(TreeReader *reader)
{
	bool goodevent=1;
//	GetBranch(reader);
	BasicJetPTmin(iteBranchJet->second,JetPTmin.back());  //Remove those Jets whose PT is not larger than the minimum requirement
	goodevent *= BasicdPhi(iteBranchJet->second,iteBranchMET->second,dPhiJMET);  // Check the dPhi between Jets and MET, Remove those Jets whose dPhi<dPhiJMET[0]. And return False if there is some jet having dPhiJMET[0]<dPhi<dPhiJMET[1]
	if(!goodevent) return goodevent;
	goodevent *= BasicLeadingPT(iteBranchJet->second,JetPTmin);  // Check the PT of Leading Jets
	if(!goodevent) return goodevent;
	BasicElePTmin(iteBranchEle->second,LepPTmin.back());  // Remove those leptons whose PT is not larger than the minimum requirement
	BasicMuonPTmin(iteBranchMuon->second,LepPTmin.back()); // Remove those leptons whose PT is not larger than the minimum requirement
	BasicdREle(iteBranchJet->second,iteBranchEle->second,dRJL); // Remove those leptons who is too close to Jets
	BasicdRMuon(iteBranchJet->second,iteBranchMuon->second,dRJL); // Remove those leptons who is too close to Jets
	goodevent *= BasicNLepSign(iteBranchEle->second,iteBranchMuon->second,NLepSign,LepPTmin);
	return goodevent;
}

void BasicCuts::BasicJetPTmin(TClonesArray * CandidatesArray, double PTmin)
{
	// Remove Those Jets from the Array whose PT is not larger than the minimum requirement
	for (int iCan = 0; iCan < CandidatesArray->GetEntriesFast();iCan++ )
	{
		if(((Jet *)CandidatesArray->At(iCan))->PT<PTmin)
		{
			CandidatesArray->RemoveAt(iCan);
		//	cout<<"Remove one!"<<endl;
		} 
	}
	CandidatesArray->Compress();
}

void BasicCuts::BasicElePTmin(TClonesArray * CandidatesArray, double PTmin)
{
	// Remove Those Jets from the Array whose PT is not larger than the minimum requirement
	for (int iCan = 0; iCan < CandidatesArray->GetEntriesFast();iCan++ )
	{
		if(((Electron *)CandidatesArray->At(iCan))->PT<PTmin)
		{
			CandidatesArray->RemoveAt(iCan);
		} 
	}
	CandidatesArray->Compress();
}

void BasicCuts::BasicMuonPTmin(TClonesArray * CandidatesArray, double PTmin)
{
	// Remove Those Jets from the Array whose PT is not larger than the minimum requirement
	for (int iCan = 0; iCan < CandidatesArray->GetEntriesFast();iCan++ )
	{
		if(((Muon *)CandidatesArray->At(iCan))->PT<PTmin)
		{
			CandidatesArray->RemoveAt(iCan);
		} 
	}
	CandidatesArray->Compress();
}

bool BasicCuts::BasicLeadingPT(TClonesArray *CandidatesArray, vector<double> PTmin)
{
	int NCandidatesMin = PTmin.size()-1;
	int NCandidatesArray = CandidatesArray->GetEntriesFast();
	if(NCandidatesArray<NCandidatesMin) return false;

	// Make Sure the PT of those Leading Jets are satisfied.
	for (int i = 0; i < NCandidatesMin; ++i)
	{
		if(((Jet *)CandidatesArray->At(i))->PT<PTmin[i]) return false;      
	}
	return true;
}

void BasicCuts::BasicdREle(TClonesArray *CandidatesArrayJet, TClonesArray *CandidatesArrayLep, double dRmin)
{
	TLorentzVector PJet,PLep;
	int NCandidatesArrayJet = CandidatesArrayJet->GetEntriesFast();
	int NCandidatesArrayLep = CandidatesArrayLep->GetEntriesFast();
	for (int iLep = 0; iLep < NCandidatesArrayLep; iLep++)
	{
		PLep = ((Electron *)CandidatesArrayLep->At(iLep))->P4();
		for (int iJet = 0; iJet < NCandidatesArrayJet; ++iJet)
		{
			PJet = ((Jet *)CandidatesArrayJet->At(iJet))->P4();
			if(PLep.DeltaR(PJet)<dRmin)
			{
				CandidatesArrayLep->RemoveAt(iLep);
				break;
			}
		}
	}
	CandidatesArrayLep->Compress();
}

void BasicCuts::BasicdRMuon(TClonesArray *CandidatesArrayJet, TClonesArray *CandidatesArrayLep, double dRmin)
{
	TLorentzVector PJet,PLep;
	int NCandidatesArrayJet = CandidatesArrayJet->GetEntriesFast();
	int NCandidatesArrayLep = CandidatesArrayLep->GetEntriesFast();
	for (int iLep = 0; iLep < NCandidatesArrayLep; iLep++)
	{
		PLep = ((Muon *)CandidatesArrayLep->At(iLep))->P4();
		for (int iJet = 0; iJet < NCandidatesArrayJet; ++iJet)
		{
			PJet = ((Jet *)CandidatesArrayJet->At(iJet))->P4();
			if(PLep.DeltaR(PJet)<dRmin)
			{
				CandidatesArrayLep->RemoveAt(iLep);
				break;
			}
		}
	}
	CandidatesArrayLep->Compress();
}

bool BasicCuts::BasicdPhi(TClonesArray *CandidatesArrayJet, const TClonesArray *CandidatesArrayMET, const double* dPhiJMET)
{
	TLorentzVector PJet,PMET;
	double dPhitemp;
	int NCandidatesArrayMET = CandidatesArrayMET->GetEntriesFast();
	int NCandidatesArrayJet = CandidatesArrayJet->GetEntriesFast();
	for (int iMET = 0; iMET < NCandidatesArrayMET; ++iMET)
	{
		PMET = ((MissingET *)CandidatesArrayMET->At(iMET))->P4();
		for (int iJet = 0; iJet < NCandidatesArrayJet; iJet++)
		{
			PJet = ((Jet *)CandidatesArrayJet->At(iJet))->P4();
			dPhitemp = fabs(PJet.DeltaPhi(PMET));
			if(dPhitemp>dPhiJMET[0] && dPhitemp<dPhiJMET[1])
			{
				return false;
			}
			else if(dPhitemp<dPhiJMET[0])
			{
				CandidatesArrayJet->RemoveAt(iJet);
			}
		}
	}
	CandidatesArrayJet->Compress();
	return true;
}

bool BasicCuts::BasicNLepSign(TClonesArray *CandidatesArrayLepEle, TClonesArray *CandidatesArrayLepMuon, const int* NLepSign, vector<double> PTmin)
{
	int NLep1 = CandidatesArrayLepEle->GetEntriesFast();
	int NLep2 = CandidatesArrayLepMuon->GetEntriesFast();
	int NLep = NLep1+NLep2;
	if (NLep!=NLepSign[0])
	{
		return false;
	}
	int totCharge=1;
	if(NLep==2)
	{
		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		{
			totCharge *= ((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge;
		}
		for (int iLep2 = 0; iLep2 < NLep2; ++iLep2)
		{
			totCharge *= ((Muon *)CandidatesArrayLepMuon->At(iLep2))->Charge;
		}
		if(totCharge!=NLepSign[1])
		{
			return false;
		}
	}
	vector<double> LepPT;
	for (int iLep = 0; iLep < NLep1; ++iLep)
	{
		LepPT.push_back(((Electron *)CandidatesArrayLepEle->At(iLep))->PT);
	}
	for (int iLep = 0; iLep < NLep2; ++iLep)
	{
		LepPT.push_back(((Muon *)CandidatesArrayLepMuon->At(iLep))->PT);
	}
	sort(LepPT.begin(),LepPT.end());
	for (int iLep = 0; iLep < LepPT.size(); ++iLep)
	{
		if(LepPT[iLep]<PTmin[iLep]) return false;
	}
	return true;
}

void BasicCuts::GetBranch(TreeReader *reader)
{
	iteBranchJet=reader->MyBranch.find(make_pair("Jet","Jet"));
	iteBranchEle=reader->MyBranch.find(make_pair("Electron","Electron"));
	iteBranchMuon=reader->MyBranch.find(make_pair("Muon","Muon"));
	iteBranchMET=reader->MyBranch.find(make_pair("MissingET","MissingET"));
	if(iteBranchJet==reader->MyBranch.end()||iteBranchEle==reader->MyBranch.end()||iteBranchMuon==reader->MyBranch.end()||iteBranchMET==reader->MyBranch.end()) cout<<"Wrong!"<<endl;
}