#include <stdio.h>
#include <stdlib.h>

#include "functions.h"


int main(int argc, char** argv) 
{
    AFN afn_a = automate_mot('a');
    AFN afn_b = automate_mot('b');

    AFN afn_r = automate_reunion(afn_a, afn_b);
    AFN afn_c = automate_concat(afn_a, afn_b);

    // dump_automate(afn_r);
    dump_automate(afn_b);



    return EXIT_SUCCESS;
}