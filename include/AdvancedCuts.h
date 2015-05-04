#ifndef ADVANCEDCUTS_h
#define ADVANCEDCUTS_h 
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <utility>
#include "Variables.h"

using namespace std;

class AdvancedCuts
{
public:
	AdvancedCuts(const char *configfile="./config/Cuts_card.dat");
	~AdvancedCuts();

	// typedef map<string,double*> TAdvancedCuts;
	typedef vector<pair<string,double*> > TAdvancedCuts;
	TAdvancedCuts MyAdCuts;
//	map<string,double*> MyAdCuts;


	void SetAdCutsFunction();
private:
	int NCuts;
	double *VariablesResults;
	typedef map<string, double(*)(TreeReader::TBranchArray)> TFuncMap;
	TFuncMap MyFuncs;

	TFuncMap::iterator *iterFunc;
};
#endif