#ifndef ADVANCEDCUTS_h
#define ADVANCEDCUTS_h 
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <utility>

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
};
#endif