#include <stdio.h>
#include <stdlib.h>

#include "functions.h"


int main(int argc, char** argv) 
{
    AFN afn = automate_mot('a');

    dump_automate(afn);

    return EXIT_SUCCESS;
}