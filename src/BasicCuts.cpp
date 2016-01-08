#include "BasicCuts.h"
#include "TLorentzVector.h"
#include <algorithm>

bool IncrPT(double a,double b)
{
	return (a>b);
}

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
				else if (tag_inner=="#"||tag_inner=="##")
				{
					config.ignore(999,'\n');
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
						else if(NameCate=="RJets")
						{
							for (int i = 0; i < Num; ++i)
							{
								config>>PTmin;
								RJetPTmin.push_back(PTmin);
							}
						}
						else if(NameCate=="BJets")
						{
							for (int i = 0; i < Num; ++i)
							{
								config>>PTmin;
								BJetPTmin.push_back(PTmin);
							}
						}
						else if(NameCate=="TauJets")
						{
							for (int i = 0; i < Num; ++i)
							{
								config>>PTmin;
								TauJetPTmin.push_back(PTmin);
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
					else if(tag_inner=="Etamax")
					{
						config>>NameCate;
						if (NameCate=="Jet")
						{
							config>>EtamaxJet;
						}
						else if (NameCate=="Lep")
						{
							config>>EtamaxLep;
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
					else if(tag_inner=="NLep&&NZ")
					{
						config>>NLepNZ[0]>>NLepNZ[1];
					}
					// else if (tag_inner=="NJetmin")
					// {
					// 	config>>NJetmin;
					// }
					// else if (tag_inner=="NBJetmin")
					// {
					// 	config>>NBJetmin;
					// }
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

bool BasicCuts::process()
{
	bool goodevent=1;
//	GetBranch(reader);
	ZLepInfo.Reset();
	BasicJetPTminEtamax(iteBranchJet->second,JetPTmin.back(),EtamaxJet);  //Remove those Jets whose PT is not larger than the minimum requirement
	goodevent *= BasicdPhi(iteBranchJet->second,iteBranchMET->second,dPhiJMET);  // Check the dPhi between Jets and MET, Remove those Jets whose dPhi<dPhiJMET[0]. And return False if there is some jet having dPhiJMET[0]<dPhi<dPhiJMET[1]
	if(!goodevent) return goodevent;
	goodevent *= BasicLeadingPT(iteBranchJet->second,JetPTmin);  // Check the PT of Leading Jets (all jets)
	if(!goodevent) return goodevent;
	goodevent *= BasicLeadingPT(iteBranchJet->second,RJetPTmin,BJetPTmin); // Check the PT of leading Regular Jets and Bjets
	if(!goodevent) return goodevent;
	BasicElePTminEtamax(iteBranchEle->second,LepPTmin.back(),EtamaxLep);  // Remove those leptons whose PT is not larger than the minimum requirement or has too large Eta
	BasicMuonPTminEtamax(iteBranchMuon->second,LepPTmin.back(),EtamaxLep); // Remove those leptons whose PT is not larger than the minimum requirement or has too large Eta
	BasicdREle(iteBranchJet->second,iteBranchEle->second,dRJL); // Remove those leptons who is too close to Jets
	BasicdRMuon(iteBranchJet->second,iteBranchMuon->second,dRJL); // Remove those leptons who is too close to Jets
	goodevent *= BasicNLepNZ(iteBranchEle->second,iteBranchMuon->second,NLepNZ,LepPTmin);
	return goodevent;
}

void BasicCuts::BasicJetPTminEtamax(TClonesArray * CandidatesArray, double PTmin, double Etamax)
{
	// Remove Those Jets from the Array whose PT is not larger than the minimum requirement
	for (int iCan = 0; iCan < CandidatesArray->GetEntriesFast();iCan++ )
	{
		if(((Jet *)CandidatesArray->At(iCan))->PT<PTmin||fabs(((Jet *)CandidatesArray->At(iCan))->Eta)>Etamax)
		{
			CandidatesArray->RemoveAt(iCan);
		//	cout<<"Remove one!"<<endl;
		} 
	}
	CandidatesArray->Compress();
}

void BasicCuts::BasicElePTminEtamax(TClonesArray * CandidatesArray, double PTmin,double Etamax)
{
	// Remove Those Jets from the Array whose PT is not larger than the minimum requirement
	for (int iCan = 0; iCan < CandidatesArray->GetEntriesFast();iCan++ )
	{
		if(((Electron *)CandidatesArray->At(iCan))->PT<PTmin||fabs(((Electron *)CandidatesArray->At(iCan))->Eta)>Etamax)
		{
			CandidatesArray->RemoveAt(iCan);
		} 
	}
	CandidatesArray->Compress();
}

void BasicCuts::BasicMuonPTminEtamax(TClonesArray * CandidatesArray, double PTmin, double Etamax)
{
	// Remove Those Jets from the Array whose PT is not larger than the minimum requirement
	for (int iCan = 0; iCan < CandidatesArray->GetEntriesFast();iCan++ )
	{
		if(((Muon *)CandidatesArray->At(iCan))->PT<PTmin||fabs(((Muon *)CandidatesArray->At(iCan))->Eta)>Etamax)
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

bool BasicCuts::BasicLeadingPT(TClonesArray *CandidatesArray, vector<double> PTRJetmin, vector<double> PTBJetmin, vector<double> PTTauJetmin)
{
	int NCandidatesRJetMin = PTRJetmin.size();
	int NCandidatesBJetMin = PTBJetmin.size();
	int NCandidatesTauJetMin = PTTauJetmin.size();
	int NCandidatesArray = CandidatesArray->GetEntriesFast();
	//if(NCandidatesArray<NCandidatesMin) return false;
	int NRJetReal = 0;
	int NBJetReal = 0;
	int NTauJetReal = 0;
	bool isBjet = 0;
	bool isTaujet = 0;
	// Make Sure the PT of those Leading Jets are satisfied.
	for (int i = 0; i < NCandidatesArray; ++i)
	{
		isBjet=(((Jet*)CandidatesArray->At(i))->BTag & (1<<0));
		isTaujet=(((Jet*)CandidatesArray->At(i))->TauTag & (1<<0));
		if (isBjet&&!isTaujet)
		{
			if (NBJetReal>=NCandidatesBJetMin)
			{
				continue;
			}
			if(((Jet *)CandidatesArray->At(i))->PT<PTBJetmin[NBJetReal]) return false;
			NBJetReal++;
		}
		else if (!isBjet&&isTaujet)
		{
			if (NTauJetReal>=NCandidatesTauJetMin)
			{
				continue;
			}
			if(((Jet *)CandidatesArray->At(i))->PT<PTTauJetmin[NTauJetReal]) return false;
			NTauJetReal++;
		}
		else
		{
			if (NRJetReal>=NCandidatesRJetMin)
			{
				continue;
			}
			if(((Jet *)CandidatesArray->At(i))->PT<PTRJetmin[NRJetReal]) return false;
			NRJetReal++;
		}		      
	}
	if (NRJetReal<NCandidatesRJetMin||NBJetReal<NCandidatesBJetMin||NTauJetReal<NCandidatesTauJetMin)
	{
		return false;
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
			if(dPhitemp>=dPhiJMET[0] && dPhitemp<dPhiJMET[1])
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

bool BasicCuts::BasicNLepNZ(TClonesArray *CandidatesArrayLepEle, TClonesArray *CandidatesArrayLepMuon, const int* NLepNZ, vector<double> PTmin)
{
	int NLep1 = CandidatesArrayLepEle->GetEntriesFast();
	int NLep2 = CandidatesArrayLepMuon->GetEntriesFast();
	int NLep = NLep1+NLep2;
	if (NLep!=NLepNZ[0])
	{
		return false;
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
	sort(LepPT.begin(),LepPT.end(),IncrPT); //Sort LepPT in Decreasing Order
	for (int iLep = 0; iLep < LepPT.size(); ++iLep)
	{
		if(LepPT[iLep]<PTmin[iLep]) return false;
	}

	ZLepInfo.SetInputArray(CandidatesArrayLepEle, CandidatesArrayLepMuon);
	if (NLepNZ[1]==-1) // Do not care about the Number of Z;
	{
		return true;
	}
	else if (NLepNZ[1]!=ZLepInfo.GetN_ZCandidates())
	{
		return false;
	}
	int Nplus=ZLepInfo.GetN_UnPair_eplus()+ZLepInfo.GetN_Unpair_muplus();
	int Nminus=ZLepInfo.GetN_UnPair_eminus()+ZLepInfo.GetN_Unpair_muminus();
	if (SS!=-1&&OS==-1)
	{
		if (!(Nplus>=2*SS||Nminus>=2*SS||(Nplus/2+Nminus/2)>=SS))
		{
			return false;
		}
	}
	else if (SS==-1&&OS!=-1)
	{
		if (Nplus<OS||Nminus<OS)
		{
			return false;
		}
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

		// int tempNZ=0;
		// if(NLep==2)
		// {
		// 	int totCharge=1;
		// 	if (NLep1==2)
		// 	{
		// 		TLorentzVector Pll;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			totCharge *= ((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge;
		// 			Pll += ((Electron *)CandidatesArrayLepEle->At(iLep1))->P4();
		// 		}
		// 		if (totCharge==-1&&(fabs(Pll.M()-91.188)<10))
		// 		{
		// 			tempNZ+=1;
		// 			ZLepInfo.Insert(0,1,0);
		// 		}
		// 	}
		// 	else if (NLep2==2)
		// 	{
		// 		TLorentzVector Pll;
		// 		for (int iLep2 = 0; iLep2 < NLep2; ++iLep2)
		// 		{
		// 			totCharge *= ((Muon *)CandidatesArrayLepMuon->At(iLep2))->Charge;
		// 			Pll += ((Muon *)CandidatesArrayLepMuon->At(iLep2))->P4();
		// 		}
		// 		if (totCharge==-1&&(fabs(Pll.M()-91.188)<10))
		// 		{
		// 			tempNZ+=1;
		// 			ZLepInfo.Insert(0,1,1);
		// 		}
		// 	}
		// 	else
		// 	{
		// 		tempNZ=0;
		// 	}
		// 	if (tempNZ!=NLepNZ[1])
		// 	{
		// 		return false;
		// 	}
		// }
		// else if(NLep==3)
		// {
		// 	if (NLep1==3)
		// 	{
		// 		vector<TLorentzVector> P4plus;
		// 		vector<TLorentzVector> P4minus;
		// 		vector<int> iLepPlus;
		// 		vector<int> iLepMinus;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			if (((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge > 0)
		// 			{
		// 				P4plus.push_back(((Electron *)CandidatesArrayLepEle->At(iLep1))->P4());
		// 				iLepPlus.push_back(iLep1);
		// 			}
		// 			else if (((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge < 0)
		// 			{
		// 				P4minus.push_back(((Electron *)CandidatesArrayLepEle->At(iLep1))->P4());
		// 				iLepMinus.push_back(iLep1);
		// 			}
		// 			else
		// 			{
		// 				cout<<"Some Stupid Wrong in BasicCuts for Z-Candidates Finding!"<<endl;
		// 			}
		// 		}
		// 		if (P4plus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iminus = 0; iminus < P4minus.size(); ++iminus)
		// 			{
		// 				Pll = P4plus[0]+P4minus[iminus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iminus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[0],iLepMinus[tempilep],0);
		// 			}
		// 		}
		// 		else if (P4minus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iplus = 0; iplus < P4plus.size(); ++iplus)
		// 			{
		// 				Pll = P4minus[0]+P4plus[iplus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iplus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[tempilep],iLepMinus[0],0);
		// 			}
		// 		}
		// 		else
		// 		{
		// 			tempNZ=0;
		// 		}	
		// 	}
		// 	else if (NLep2==3)
		// 	{
		// 		vector<TLorentzVector> P4plus;
		// 		vector<TLorentzVector> P4minus;
		// 		vector<int> iLepPlus;
		// 		vector<int> iLepMinus;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			if (((Muon *)CandidatesArrayLepMuon->At(iLep1))->Charge > 0)
		// 			{
		// 				P4plus.push_back(((Muon *)CandidatesArrayLepMuon->At(iLep1))->P4());
		// 				iLepPlus.push_back(iLep1);
		// 			}
		// 			else if (((Muon *)CandidatesArrayLepMuon->At(iLep1))->Charge < 0)
		// 			{
		// 				P4minus.push_back(((Muon *)CandidatesArrayLepMuon->At(iLep1))->P4());
		// 				iLepMinus.push_back(iLep1);
		// 			}
		// 			else
		// 			{
		// 				cout<<"Some Stupid Wrong in BasicCuts for Z-Candidates Finding!"<<endl;
		// 			}
		// 		}
		// 		if (P4plus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iminus = 0; iminus < P4minus.size(); ++iminus)
		// 			{
		// 				Pll = P4plus[0]+P4minus[iminus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iminus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[0],iLepMinus[tempilep],1);
		// 			}
		// 		}
		// 		else if (P4minus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iplus = 0; iplus < P4plus.size(); ++iplus)
		// 			{
		// 				Pll = P4minus[0]+P4plus[iplus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iplus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[tempilep],iLepMinus[0],1);
		// 			}
		// 		}
		// 		else
		// 		{
		// 			tempNZ=0;
		// 		}
		// 	}
		// 	else if (NLep1==2&&NLep2==1)
		// 	{
		// 		TLorentzVector Pll;
		// 		int totCharge=1;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			totCharge *= ((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge;
		// 			Pll += ((Electron *)CandidatesArrayLepEle->At(iLep1))->P4();
		// 		}
		// 		if (totCharge==-1&&(fabs(Pll.M()-91.188)<10))
		// 		{
		// 			tempNZ+=1;
		// 			ZLepInfo.Insert(0,1,0);
		// 		}
		// 	}
		// 	else if (NLep1==1&&NLep2==2)
		// 	{
		// 		TLorentzVector Pll;
		// 		int totCharge=1;
		// 		for (int iLep2 = 0; iLep2 < NLep2; ++iLep2)
		// 		{
		// 			totCharge *= ((Muon *)CandidatesArrayLepMuon->At(iLep2))->Charge;
		// 			Pll += ((Muon *)CandidatesArrayLepMuon->At(iLep2))->P4();
		// 		}
		// 		if (totCharge==-1&&(fabs(Pll.M()-91.188)<10))
		// 		{
		// 			tempNZ+=1;
		// 			ZLepInfo.Insert(0,1,1);
		// 		}
		// 	}
		// 	else
		// 	{
		// 		tempNZ=0;
		// 	}

		// 	if (tempNZ!=NLepNZ[1])
		// 	{
		// 		return false;
		// 	}
		// }
		// else if (NLep==4)
		// {
		// 	if (NLep1==4)
		// 	{
		// 		vector<TLorentzVector> P4plus;
		// 		vector<TLorentzVector> P4minus;
		// 		vector<int> iLepPlus;
		// 		vector<int> iLepMinus;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			if (((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge > 0)
		// 			{
		// 				P4plus.push_back(((Electron *)CandidatesArrayLepEle->At(iLep1))->P4());
		// 				iLepPlus.push_back(iLep1);
		// 			}
		// 			else if (((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge < 0)
		// 			{
		// 				P4minus.push_back(((Electron *)CandidatesArrayLepEle->At(iLep1))->P4());
		// 				iLepMinus.push_back(iLep1);
		// 			}
		// 			else
		// 			{
		// 				cout<<"Some Stupid Wrong in BasicCuts for Z-Candidates Finding!"<<endl;
		// 			}
		// 		}
		// 		if (P4plus.size()>=1&&P4plus.size()<=3)
		// 		{
		// 			for (int iplus = 0; iplus < P4plus.size(); ++iplus)
		// 			{
		// 				double tempdelta=1000000;
		// 				int tempilep=999;
		// 				TLorentzVector Pll;
		// 				for (int iminus = 0; iminus < P4minus.size(); ++iminus)
		// 				{
		// 					Pll = P4plus[iplus]+P4minus[iminus];
		// 					if(fabs(Pll.M()-91.188)<tempdelta)
		// 					{
		// 						tempdelta=fabs(Pll.M()-91.188);
		// 						tempilep=iminus;
		// 					}
		// 				}
		// 				if (tempilep!=999&&tempdelta<10)
		// 				{
		// 					tempNZ+=1;
		// 					ZLepInfo.Insert(iLepPlus[iplus],iLepMinus[tempilep],0);
		// 				}
		// 			}
		// 		}
		// 	}
		// 	else if (NLep2==4)
		// 	{
		// 		vector<TLorentzVector> P4plus;
		// 		vector<TLorentzVector> P4minus;
		// 		vector<int> iLepPlus;
		// 		vector<int> iLepMinus;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			if (((Muon *)CandidatesArrayLepMuon->At(iLep1))->Charge > 0)
		// 			{
		// 				P4plus.push_back(((Muon *)CandidatesArrayLepMuon->At(iLep1))->P4());
		// 				iLepPlus.push_back(iLep1);
		// 			}
		// 			else if (((Muon *)CandidatesArrayLepMuon->At(iLep1))->Charge < 0)
		// 			{
		// 				P4minus.push_back(((Muon *)CandidatesArrayLepMuon->At(iLep1))->P4());
		// 				iLepMinus.push_back(iLep1);
		// 			}
		// 			else
		// 			{
		// 				cout<<"Some Stupid Wrong in BasicCuts for Z-Candidates Finding!"<<endl;
		// 			}
		// 		}
		// 		if (P4plus.size()>=1&&P4plus.size()<=3)
		// 		{
		// 			for (int iplus = 0; iplus < P4plus.size(); ++iplus)
		// 			{
		// 				double tempdelta=1000000;
		// 				int tempilep=999;
		// 				TLorentzVector Pll;
		// 				for (int iminus = 0; iminus < P4minus.size(); ++iminus)
		// 				{
		// 					Pll = P4plus[iplus]+P4minus[iminus];
		// 					if(fabs(Pll.M()-91.188)<tempdelta)
		// 					{
		// 						tempdelta=fabs(Pll.M()-91.188);
		// 						tempilep=iminus;
		// 					}
		// 				}
		// 				if (tempilep!=999&&tempdelta<10)
		// 				{
		// 					tempNZ+=1;
		// 					ZLepInfo.Insert(iLepPlus[iplus],iLepMinus[tempilep],1);
		// 				}
		// 			}
		// 		}
		// 	}
		// 	else if (NLep1==3&&NLep2==1)
		// 	{
		// 		vector<TLorentzVector> P4plus;
		// 		vector<TLorentzVector> P4minus;
		// 		vector<int> iLepPlus;
		// 		vector<int> iLepMinus;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			if (((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge > 0)
		// 			{
		// 				P4plus.push_back(((Electron *)CandidatesArrayLepEle->At(iLep1))->P4());
		// 				iLepPlus.push_back(iLep1);
		// 			}
		// 			else if (((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge < 0)
		// 			{
		// 				P4minus.push_back(((Electron *)CandidatesArrayLepEle->At(iLep1))->P4());
		// 				iLepMinus.push_back(iLep1);
		// 			}
		// 			else
		// 			{
		// 				cout<<"Some Stupid Wrong in BasicCuts for Z-Candidates Finding!"<<endl;
		// 			}
		// 		}
		// 		if (P4plus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iminus = 0; iminus < P4minus.size(); ++iminus)
		// 			{
		// 				Pll = P4plus[0]+P4minus[iminus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iminus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[0],iLepMinus[tempilep],0);
		// 			}
		// 		}
		// 		else if (P4minus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iplus = 0; iplus < P4plus.size(); ++iplus)
		// 			{
		// 				Pll = P4minus[0]+P4plus[iplus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iplus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[tempilep],iLepMinus[0],0);
		// 			}
		// 		}
		// 		else
		// 		{
		// 			tempNZ=0;
		// 		}
		// 	}
		// 	else if (NLep1==1&&NLep2==3)
		// 	{
		// 		vector<TLorentzVector> P4plus;
		// 		vector<TLorentzVector> P4minus;
		// 		vector<int> iLepPlus;
		// 		vector<int> iLepMinus;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			if (((Muon *)CandidatesArrayLepMuon->At(iLep1))->Charge > 0)
		// 			{
		// 				P4plus.push_back(((Muon *)CandidatesArrayLepMuon->At(iLep1))->P4());
		// 				iLepPlus.push_back(iLep1);
		// 			}
		// 			else if (((Muon *)CandidatesArrayLepMuon->At(iLep1))->Charge < 0)
		// 			{
		// 				P4minus.push_back(((Muon *)CandidatesArrayLepMuon->At(iLep1))->P4());
		// 				iLepMinus.push_back(iLep1);
		// 			}
		// 			else
		// 			{
		// 				cout<<"Some Stupid Wrong in BasicCuts for Z-Candidates Finding!"<<endl;
		// 			}
		// 		}
		// 		if (P4plus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iminus = 0; iminus < P4minus.size(); ++iminus)
		// 			{
		// 				Pll = P4plus[0]+P4minus[iminus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iminus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[0],iLepMinus[tempilep],1);
		// 			}
		// 		}
		// 		else if (P4minus.size()==1)
		// 		{
		// 			double tempdelta=1000000;
		// 			int tempilep=999;
		// 			TLorentzVector Pll;
		// 			for (int iplus = 0; iplus < P4plus.size(); ++iplus)
		// 			{
		// 				Pll = P4minus[0]+P4plus[iplus];
		// 				if(fabs(Pll.M()-91.188)<tempdelta)
		// 				{
		// 					tempdelta=fabs(Pll.M()-91.188);
		// 					tempilep=iplus;
		// 				}
		// 			}
		// 			if (tempilep!=999&&tempdelta<10)
		// 			{
		// 				tempNZ+=1;
		// 				ZLepInfo.Insert(iLepPlus[tempilep],iLepMinus[0],1);
		// 			}
		// 		}
		// 		else
		// 		{
		// 			tempNZ=0;
		// 		}
		// 	}
		// 	else if (NLep1==2&&NLep2==2)
		// 	{
		// 		TLorentzVector Pll1;
		// 		int totCharge1=1;
		// 		for (int iLep1 = 0; iLep1 < NLep1; ++iLep1)
		// 		{
		// 			totCharge1 *= ((Electron *)CandidatesArrayLepEle->At(iLep1))->Charge;
		// 			Pll1 += ((Electron *)CandidatesArrayLepEle->At(iLep1))->P4();
		// 		}
		// 		if (totCharge1==-1&&(fabs(Pll1.M()-91.188)<10))
		// 		{
		// 			tempNZ+=1;
		// 			ZLepInfo.Insert(0,1,0);
		// 		}
		// 		TLorentzVector Pll2;
		// 		int totCharge2=1;
		// 		for (int iLep2 = 0; iLep2 < NLep2; ++iLep2)
		// 		{
		// 			totCharge2 *= ((Muon *)CandidatesArrayLepMuon->At(iLep2))->Charge;
		// 			Pll2 += ((Muon *)CandidatesArrayLepMuon->At(iLep2))->P4();
		// 		}
		// 		if (totCharge2==-1&&(fabs(Pll2.M()-91.188)<10))
		// 		{
		// 			tempNZ+=1;
		// 			ZLepInfo.Insert(0,1,1);
		// 		}
		// 	}

		// 	if (tempNZ!=NLepNZ[1])
		// 	{
		// 		return false;
		// 	}
		// }
		// int NEplus=0;
	// int NEminus=0;
	// int NMuplus=0;
	// int NMuminus=0;
	// for (int iLep = 0; iLep < NLep1; ++iLep)
	// {
	// 	int good=1;
	// 	for (int iZ = 0; iZ < ZLepInfo.GetNZ(); ++iZ)
	// 	{
	// 		if (ZLepInfo.GetZLepFlavor(iZ)==0&&(iLep==ZLepInfo.GetZLepInfo1(iZ)||iLep==ZLepInfo.GetZLepInfo2(iZ)))
	// 		{
	// 			good=0;
	// 		}
	// 	}
	// 	if (good==0) continue;
	// 	if (((Electron *)CandidatesArrayLepEle->At(iLep))->Charge == 1)
	// 	{
	// 		NEplus++;
	// 	}
	// 	if (((Electron *)CandidatesArrayLepEle->At(iLep))->Charge == -1)
	// 	{
	// 		NEminus++;
	// 	}
	// }
	// for (int iLep = 0; iLep < NLep2; ++iLep)
	// {
	// 	int good=1;
	// 	for (int iZ = 0; iZ < ZLepInfo.GetNZ(); ++iZ)
	// 	{
	// 		if (ZLepInfo.GetZLepFlavor(iZ)==1&&(iLep==ZLepInfo.GetZLepInfo1(iZ)||iLep==ZLepInfo.GetZLepInfo2(iZ)))
	// 		{
	// 			good=0;
	// 		}
	// 	}
	// 	if (good==0) continue;
	// 	if (((Muon *)CandidatesArrayLepMuon->At(iLep))->Charge == 1)
	// 	{
	// 		NMuplus++;
	// 	}
	// 	if (((Muon *)CandidatesArrayLepMuon->At(iLep))->Charge == -1)
	// 	{
	// 		NMuminus++;
	// 	}
	// }
