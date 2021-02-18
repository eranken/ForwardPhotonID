#include <stdlib.h>
#include <TSystem.h>
#include "lib/hoeFit.C"

void runHoE() {
	int N_Regions=3;
	for (size_t i = 0; i < N_Regions; i++) {
		hoeFit(i);
	}
}
