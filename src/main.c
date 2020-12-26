#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "functions.h"

int main(int argc, char** argv) 
{
    srand(time(NULL));

    AFN afn_a = automate_mot('a');

    AFN afn_a_s = automate_kleene(afn_a);

    afficherAFN(afn_a_s);

    AFD afd_r = determiniser_AFN(afn_a_s);
    afficherAFD(afd_r);

    AFD afd_r_m = minimiser_AFD(afd_r);
    afficherAFD(afd_r_m);

    execute_AFD(afd_r, "aaaa");

    // TODO: free automates

    return EXIT_SUCCESS;
}