#include <cstdlib>
#include <TMath.h>
#include "lib/scr99.C"
#include "lib/configParser.h"

void run99(){
        ConfigParser* CP = new ConfigParser("config.cfg");
        vector<double> etaRegionBins = CP->GetVector<double>("regions");
    
        int N_Regions=0;
        for (size_t i = 0; i < etaRegionBins.size() ; i+=2)
        {
                N_Regions++;
        }

	for (int i=0; i<N_Regions; i++) {
		scr99(i,60);
	}
	  
}
