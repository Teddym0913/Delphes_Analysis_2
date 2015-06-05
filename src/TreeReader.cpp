#include <iostream>
#include "TreeReader.h"
#include <fstream>

using namespace std;

TreeReader::TreeReader(const char *configfile)
{
	ifstream config(configfile,ios::in);
	string temp,a,b,Bname,Bclass;
	while(config)
	{
		config>>temp;
        if(temp=="#")
        { 
        	config.ignore(999,'\n');
        }
        else if(temp=="add")
        {
           config>>a;
            if(a=="Branch")
            {
                config>>b>>Bname>>Bclass;
                config.ignore(999,'\n');
                BranchInfo.push_back(make_pair(Bname,Bclass));
            }
        }
        else
        {
        	config.ignore(999,'\n');
        }
	}

}

TreeReader::~TreeReader()
{
	TBranchArray::iterator itMyBranch;
	for(itMyBranch=MyBranch.begin();itMyBranch!=MyBranch.end();++itMyBranch)
	{
		delete itMyBranch->second;
	}
}

bool TreeReader::SetTreeChain(TChain *chain)
{
	bool Goodness = 1;
	reader = new ExRootTreeReader(chain);
	for (int Nbranch = 0; Nbranch < BranchInfo.size(); ++Nbranch)
	{
		TClonesArray *branchtemp = reader->UseBranch(BranchInfo[Nbranch].first.c_str());
		if(branchtemp)
		{
			MyBranch.insert(make_pair(BranchInfo[Nbranch],branchtemp));
		}
		else
		{
			cout<<"Check Your Root file, the Branch named "<<BranchInfo[Nbranch].first<<" does not Exist!"<<endl;
			Goodness *= 0;
		}
	}
	TClonesArray *branchEventtemp = reader->UseBranch("Event");
	MyBranch.insert(make_pair(make_pair("Event","LHEFEvent"),branchEventtemp));
	return Goodness;

}


bool TreeReader::GetEntry(Long64_t entry)
{
	bool good=reader->ReadEntry(entry);
    return good;
}