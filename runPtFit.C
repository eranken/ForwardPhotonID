#include <stdlib.h>
#include <TSystem.h>
#include "ptFit.C"

void runPtFit() {
	int N_Regions=4;
	for (size_t i = 0; i < N_Regions; i++) {
		ptFit("P","lin",i);
		ptFit("N","lin",i);
	}
}
