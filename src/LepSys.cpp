#include "LepSys.h"
#include <algorithm>

ZCandidatesInfo::ZCandidatesInfo()
{
    NZ=0;
}

ZCandidatesInfo::~ZCandidatesInfo()
{

}

void ZCandidatesInfo::Reset()
{
    Lep1.clear();
    Lep2.clear();
    LepFlavor.clear();
    NZ=0;
}

void ZCandidatesInfo::Insert(int lep1, int lep2, int lepflavor) // lepflavor 0 for electron 1 for muon
{
    Lep1.push_back(lep1);
    Lep2.push_back(lep2);
    LepFlavor.push_back(lepflavor);
    NZ+=1;
}

int ZCandidatesInfo::GetNZ()
{
    return NZ;
}

int ZCandidatesInfo::GetZLepInfo1(int iZ) // Starts From 0 to NZ-1
{
    return Lep1[iZ];
}
int ZCandidatesInfo::GetZLepInfo2(int iZ) // Starts From 0 to NZ-1
{
    return Lep2[iZ];
}
int ZCandidatesInfo::GetZLepFlavor(int iZ)
{
    return LepFlavor[iZ];
}

LepSysInfo::LepSysInfo()
{
    N_Leps=0;
    N_ZCandidates=0;
    N_UnPairLeps=0;

    N_Elec=0;
    N_Muon=0;
}
LepSysInfo::~LepSysInfo()
{}

int LepSysInfo::GetN_Leps()
{
    return N_Leps;
}
int LepSysInfo::GetN_ZCandidates()
{
    return N_ZCandidates;
}
int LepSysInfo::GetN_UnPairLeps()
{
    return N_UnPairLeps;
}
int LepSysInfo::GetFlavor_ZCandi(int iZ) // 0 for e, 1 for mu
{
    return Flavor_ZCandi[iZ];
}
int LepSysInfo::GetIndex_Lep1_ZCandi(int iZ) // The Index in Original Input Array
{
    return Index_Lep1_ZCandi[iZ];
}
int LepSysInfo::GetIndex_Lep2_ZCandi(int iZ)
{
    return Index_Lep2_ZCandi[iZ];
}
int LepSysInfo::GetN_UnPair_eplus()
{
    return Index_Unpair_eplus.size();
}
int LepSysInfo::GetN_UnPair_eminus()
{
    return Index_Unpair_eminus.size();
}
int LepSysInfo::GetN_Unpair_muplus()
{
    return Index_Unpair_muplus.size();
}
int LepSysInfo::GetN_Unpair_muminus()
{
    return Index_Unpair_muminus.size();
}
int LepSysInfo::GetIndex_Unpair_eplus(int ieplus) // The Index of int-th unpair e+ in Original Input Array
{
    return Index_Unpair_eplus[ieplus];
}
int LepSysInfo::GetIndex_Unpair_eminus(int ieminus)
{
    return Index_Unpair_eminus[ieminus];
}
int LepSysInfo::GetIndex_Unpair_muplus(int imuplus)
{
    return Index_Unpair_muplus[imuplus];
}
int LepSysInfo::GetIndex_Unpair_muminus(int imuminus)
{
    return Index_Unpair_muminus[imuminus];
}
void LepSysInfo::Reset()
{
    N_Leps=0;
    N_ZCandidates=0;
    N_UnPairLeps=0;

    N_Elec=0;
    N_Muon=0;

    Flavor_ZCandi.clear();
    Index_Lep1_ZCandi.clear();
    Index_Lep2_ZCandi.clear();
    Index_Unpair_eplus.clear();
    Index_Unpair_eminus.clear();
    Index_Unpair_muplus.clear();
    Index_Unpair_muminus.clear();
}
void LepSysInfo::SetInputArray(TClonesArray *CandidatesArrayLepEle, TClonesArray *CandidatesArrayLepMuon)
{
    N_Elec=CandidatesArrayLepEle->GetEntriesFast();
    N_Muon=CandidatesArrayLepMuon->GetEntriesFast();
    N_Leps=N_Muon+N_Elec;

    // vector<int> eplus_index;
    // vector<int> eminus_index;
    for (int iEle = 0; iEle < N_Elec; ++iEle)
    {
        if (((Electron *)CandidatesArrayLepEle->At(iEle))->Charge==1)
        {
            Index_Unpair_eplus.push_back(iEle);
        }
        else if (((Electron *)CandidatesArrayLepEle->At(iEle))->Charge==-1)
        {
            Index_Unpair_eminus.push_back(iEle);
        }
    }
    // vector<int> muplus_index;
    // vector<int> muminus_index;
    for (int iMuon = 0; iMuon < N_Muon; ++iMuon)
    {
        if (((Muon *)CandidatesArrayLepMuon->At(iMuon))->Charge==1)
        {
            Index_Unpair_muplus.push_back(iMuon);
        }
        else if (((Muon *)CandidatesArrayLepMuon->At(iMuon))->Charge==-1)
        {
            Index_Unpair_muminus.push_back(iMuon);
        }
    }

    double tempdelta;
    int tempiminus;
    TLorentzVector Pplus,Pll;
    for (int iEplus = 0; iEplus < Index_Unpair_eplus.size();)
    {
        tempdelta=1000000;
        tempiminus=999;
        Pplus = ((Electron *)CandidatesArrayLepEle->At(Index_Unpair_eplus[iEplus]))->P4();
        for (int iEminus = 0; iEminus < Index_Unpair_eminus.size(); ++iEminus)
        {
            Pll =  Pplus + ((Electron *)CandidatesArrayLepEle->At(Index_Unpair_eminus[iEminus]))->P4();
            if (fabs(Pll.M()-91.188)<tempdelta)
            {
                tempdelta=fabs(Pll.M()-91.188);
                tempiminus=iEminus;
            }
        }
        if (tempdelta<10&&tempiminus!=999)
        {
            Flavor_ZCandi.push_back(0);
            Index_Lep1_ZCandi.push_back(Index_Unpair_eplus[iEplus]);
            Index_Lep2_ZCandi.push_back(Index_Unpair_eminus[tempiminus]);
            N_ZCandidates++;
            Index_Unpair_eminus.erase(Index_Unpair_eminus.begin()+tempiminus);
            Index_Unpair_eplus.erase(Index_Unpair_eplus.begin()+iEplus);
        }
        else
        {
            iEplus++;
        }
    }
    for (int iMuplus = 0; iMuplus < Index_Unpair_muplus.size(); )
    {
        tempdelta=1000000;
        tempiminus=999;
        Pplus = ((Muon *)CandidatesArrayLepMuon->At(Index_Unpair_muplus[iMuplus]))->P4();
        //for (int iMuminus = 0; iMuminus < Index_Unpair_muminus.size(); ++iMuminus)
        for (int iMuminus = 0; iMuminus < Index_Unpair_muminus.size(); ++iMuminus )
        {
            Pll = Pplus + ((Muon *)CandidatesArrayLepMuon->At(Index_Unpair_muminus[iMuminus]))->P4();
            if (fabs(Pll.M()-91.188)<tempdelta)
            {
                tempdelta=fabs(Pll.M()-91.188);
                tempiminus=iMuminus;
            }
        }
        if (tempdelta<10&&tempiminus!=999)
        {
            Flavor_ZCandi.push_back(1);
            Index_Lep1_ZCandi.push_back(Index_Unpair_muplus[iMuplus]);
            Index_Lep2_ZCandi.push_back(Index_Unpair_muminus[tempiminus]);
            N_ZCandidates++;
            Index_Unpair_muminus.erase(Index_Unpair_muminus.begin()+tempiminus);
            Index_Unpair_muplus.erase(Index_Unpair_muplus.begin()+iMuplus);
        }
        else
        {
            iMuplus++;
        }
    }
}


