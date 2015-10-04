#include "AdvancedCuts.h"
#include <fstream>

AdvancedCuts::AdvancedCuts(const char *configfile)
{
	ifstream config(configfile,ios::in);
	string tag,tag_inner,cName;
	int tempcuts;
	while(config)
	{
		config>>tag;
		if(tag=="Advanced")
		{
			config.ignore(999,'\n');
			while(config)
			{
				config>>tag_inner;
				if(tag_inner=="NCuts")
				{
					config>>NCuts;
					config.ignore(999,'\n');
					for (int i = 0; i < NCuts; ++i)
					{
                        int NFollow;
						//config>>cName>>CutsInfo[0]>>CutsInfo[1]>>CutsInfo[2];
						config>>cName>>NFollow;
						vector<double> CutsInfo;
						for (int iFollow = 0; iFollow < NFollow; ++iFollow)
						{
							config>>tempcuts;
							CutsInfo.push_back(tempcuts);
						}
						config.ignore(999,'\n');
						MyAdCuts.push_back(make_pair(cName,CutsInfo));
                        //MyAdCuts.insert(make_pair(cName,CutsInfo));
					}
				}
				else if(tag_inner=="}")
				{
					break;
				}
                else
                {
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

AdvancedCuts::~AdvancedCuts()
{}

// void AdvancedCuts::SetAdCutsFunction()
// {
// 	VariablesResults = new double[NCuts];
// 	iterFunc = new TFuncMap::iterator[NCuts];
// 	for (int i = 0; i < NCuts; ++i)
// 	{
// 		iterFunc[i] = MyFuncs.find(MyAdCuts[i].first);
// 	}

// }