#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

/**
 * Cette fonction permet de renvoyer un automate
 * standard reconnaissant le langage vide.
 * 
 * @return un AFN reconnaisant le langage vide
 **/ 
AFN automate_vide() {
    AFN automate;

    // Initialisation des états de l'automate
    automate.q_size = 1;
    automate.q = (uint*) malloc(sizeof(uint));
    *(automate.q) = 0;

    // Initialisation de l'alphabet
    automate.alphabet = '\0';

    // Initialisation de l'état initial de l'automate
    automate.s = 0;
    
    // Initialisation des états finaux de l'automate
    automate.f_size = 1;
    automate.f = (uint*) malloc(sizeof(uint));
    *(automate.f) = 0;

    // Initialisation des transitions de l'automate
    automate.delta_size = 0;
    automate.delta = NULL;

    return automate;
}

/**
 * Cette fonction permet de renvoyer un automate
 * standard reconnaissant un mot d'un caractere donnée.
 * 
 * @param caractere: le caractere d'un mot qlcq
 * @return un AFN reconnaisant le langage vide
 **/ 
AFN automate_mot(char caractere) {
    AFN automate;

    // Initialisation des états de l'automate
    automate.q_size = 2;
    automate.q = (uint*) malloc(sizeof(uint) * 2);
    *(automate.q) = 0; *(automate.q+1) = 1;

    // Initialisation de l'alphabet
    automate.alphabet = '\0';

    // Initialisation de l'état initial de l'automate
    automate.s = 0;
    
    // Initialisation des états finaux de l'automate
    automate.f_size = 1;
    automate.f = (uint*) malloc(sizeof(uint));
    *(automate.f) = 1;

    // Initialisation des transitions de l'automate
    automate.delta_size = 1;
    automate.delta = (Transition*) malloc(sizeof(Transition));
    (automate.delta)->e1 = 0;
    (automate.delta)->e2 = 1;
    (automate.delta)->alphabet = caractere;

    return automate;
}

void dump_automate(AFN automate) {
    size_t i;
    printf("Q = {");
    for (i = 0; i < automate.q_size; i++)
        printf("%d, ", *(automate.q+i));
    printf("}\n");

    printf("s = %d\n", automate.s);

    printf("F = {");
    for (i = 0; i < automate.f_size; i++)
        printf("%d, ", *(automate.f+i));
    printf("}\n");

    printf("delta = {");
    for (i = 0; i < automate.delta_size; i++)
        printf("(%d, %c, %d), ", 
            (automate.delta+i)->e1, 
            (automate.delta+i)->alphabet, 
            (automate.delta+i)->e2);
    printf("}\n");
}

