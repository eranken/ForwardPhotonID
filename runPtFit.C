#include <stdlib.h>
#include <TSystem.h>
#include "lib/ptFit.C"
#include "lib/configParser.h"

void runPtFit() {
	        ConfigParser* CP = new ConfigParser("config.cfg");
        vector<double> etaRegionBins = CP->GetVector<double>("regions");
	

	int N_Regions=0;
	for (size_t i = 0; i < etaRegionBins.size() ; i+=2)
	{
		N_Regions++;
	}

	string isoPfit = CP->Get<string>("isoPfit");
	string isoNfit = CP->Get<string>("isoNfit");



	for (size_t i = 0; i < N_Regions; i++) {
		ptFit("N",isoNfit,i);
		ptFit("P",isoPfit,i);
	}
}
