#include <stdlib.h>
#include <TSystem.h>
#include "ptFit.C"

void runPtFit() {
	ptFit("P","lin",1);
	ptFit("N","lin",1);
	ptFit("P","lin",2);
	ptFit("N","lin",2);
}
