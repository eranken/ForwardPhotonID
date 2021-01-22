#include <stdlib.h>
#include <TSystem.h>
#include "ptFit.C"

void runPtFit() {
	int N_Regions=6;
	for (size_t i = 4; i < N_Regions; i++) {
		ptFit("N","quad",i);
		if (i<1) {
			ptFit("P","lin",i);
		}
		else ptFit("P","log",i);
	}
}
