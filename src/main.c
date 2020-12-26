#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "functions.h"

int main(int argc, char** argv) 
{
    srand(time(NULL));

    AFN afn_a = automate_mot('a');
    AFN afn_b = automate_mot('b');

    AFN afn_r = automate_reunion(afn_a, afn_b);

    AFD afd_r = determiniser_AFN(afn_r);
    afficherAFD(afd_r);

    AFD afd_r_min = minimiser_AFN(afd_r);
    afficherAFD(afd_r_min);

    // TODO: free automates

    return EXIT_SUCCESS;
}