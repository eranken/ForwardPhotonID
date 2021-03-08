#include <stdlib.h>
#include <TSystem.h>
#include "lib/hoeFit.C"
#include "lib/configParser.h"

void runHoE() {
        ConfigParser* CP = new ConfigParser("config.cfg");
        vector<double> etaRegionBins = CP->GetVector<double>("regions");
    

        int N_Regions=0;
        for (size_t i = 0; i < etaRegionBins.size() ; i+=2)
        {
                N_Regions++;
        }

	for (size_t i = 0; i < N_Regions; i++) {
		hoeFit(i);
	}
}
