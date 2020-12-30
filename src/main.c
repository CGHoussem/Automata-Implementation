#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "functions.h"

int main(int argc, char** argv) 
{
    srand(time(NULL));

    AFN afn_a = automate_mot('a');
    AFN afn_a_s = automate_kleene(afn_a);
    AFN afn_b = automate_mot('b');
    AFN afn_b_s = automate_kleene(afn_b);

    AFN afn_r = automate_concat(afn_a_s, afn_b_s);
    printf("\nAFN (a*b*)\n");
    afficherAFN(afn_r);

    printf("\nAFN determinisé\n");
    AFD afd_r = determiniser_AFN(afn_r);
    afficherAFD(afd_r);

    printf("\nAFD minimisé\n");
    AFD afd_r_m = minimiser_AFD(afd_r);
    afficherAFD(afd_r_m);

    execute_AFD(afd_r, "aaaab");
    execute_AFD(afd_r, "baa");
    execute_AFD(afd_r, "aaba");
    execute_AFD(afd_r, "aaabbb");
    execute_AFD(afd_r, "aass");

    // TODO: free automates

    return EXIT_SUCCESS;
}