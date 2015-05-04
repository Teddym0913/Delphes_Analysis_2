#ifndef Variables_h
#define Variables_h

#include "TreeReader.h"
#include "modules/Delphes.h"
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"

using namespace std;

double MET(TreeReader::TBranchArray);

double HT(TreeReader::TBranchArray);

double MT(TreeReader::TBranchArray);

double dMLL(TreeReader::TBranchArray);

double MT2(TreeReader::TBranchArray);

double NJets(TreeReader::TBranchArray);

double NBjets(TreeReader::TBranchArray);



#endif