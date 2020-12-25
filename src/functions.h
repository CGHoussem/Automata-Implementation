#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

AFN automate_vide();
AFN automate_mot_vide();
AFN automate_mot(char);
AFN automate_reunion(AFN, AFN);
AFN automate_concat(AFN, AFN);
AFN automate_kleene(AFN);

void execute_AFD(AFD, char*);
AFD determiniser_AFN(AFN);
AFD minimiser_AFN(AFD);

void afficherAFN(AFN);
void afficherAFD(AFD);

#endif
