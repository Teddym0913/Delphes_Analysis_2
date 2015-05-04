#include "Variables.h"

double MET(TreeReader::TBranchArray myBranchArray)
{
	TreeReader::TBranchArray::iterator iteBranch = myBranchArray.find(make_pair("MissingET","MissingET"));
	return ((MissingET *)iteBranch->second->At(0))->MET;
}

double HT(TreeReader::TBranchArray myBranchArray)
{
	
}

double Meff(TreeReader::TBranchArray myBranchArray)
{
	
}