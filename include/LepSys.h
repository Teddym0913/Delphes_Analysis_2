#ifndef LEPSYS_H
#define LEPSYS_H
#include <string>
#include <vector>
#include <fstream>
#include "TreeReader.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

class ZCandidatesInfo
{
public:
    ZCandidatesInfo();
    ~ZCandidatesInfo();

    void Reset();
    void Insert(int,int,int);
    int GetNZ();
    int GetZLepInfo1(int);
    int GetZLepInfo2(int);
    int GetZLepFlavor(int);

private:
    int NZ;
    vector<int> LepFlavor;
    vector<int> Lep1;
    vector<int> Lep2;
};

class LepSysInfo
{
public:
    LepSysInfo();
    ~LepSysInfo();

    int GetN_Leps();
    int GetN_ZCandidates();
    int GetN_UnPairLeps();
    int GetFlavor_ZCandi(int); // 0 for e, 1 for mu
    int GetIndex_Lep1_ZCandi(int); // The Index in Original Input Array
    int GetIndex_Lep2_ZCandi(int); 
    int GetN_UnPair_eplus();
    int GetN_UnPair_eminus();
    int GetN_Unpair_muplus();
    int GetN_Unpair_muminus();
    int GetIndex_Unpair_eplus(int); // The Index of int-th unpair e+ in Original Input Array
    int GetIndex_Unpair_eminus(int);
    int GetIndex_Unpair_muplus(int);
    int GetIndex_Unpair_muminus(int);

    void SetInputArray(TClonesArray*, TClonesArray*);
    void Reset();

    // int GetNpair_SSOF();
    // int GetNpair_SSSF();
    // int GetNpair_OSOF();
    // int GetNpair_OSSF();
    // int GetNpair_SS();
    // int GetNpair_OS();
    // int GetNpair_OF();
    // int GetNpair_SF();
private:
    int N_Leps;
    int N_ZCandidates;
    int N_UnPairLeps;
    
    int N_Elec;
    int N_Muon;
    
    vector<int> Flavor_ZCandi;
    vector<int> Index_Lep1_ZCandi;
    vector<int> Index_Lep2_ZCandi;

    vector<int> Index_Unpair_eplus;
    vector<int> Index_Unpair_eminus;
    vector<int> Index_Unpair_muplus;
    vector<int> Index_Unpair_muminus;

};

#endif