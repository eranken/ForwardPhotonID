#include <stdlib.h>
#include <TSystem.h>
#include "ptFit.C"

void runPtFit() {
	int N_Regions=4;
	for (size_t i = 0; i < N_Regions; i++) {
		ptFit("N","quad",i);
		if (i==0) {
			ptFit("P","lin",i);
		}
		else ptFit("P","log",i);
	}
}
