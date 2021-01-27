#include <stdlib.h>
#include <TSystem.h>
#include "lib/ptFit.C"

void runPtFit() {
	int N_Regions=5;
	for (size_t i = 0; i < N_Regions; i++) {
		ptFit("N","quad",i);
		if (false) {
			ptFit("P","lin",i);
		}
		else ptFit("P","log",i);
	}
}
