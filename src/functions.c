#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

// TODO: Check for alphabet use in the structure

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

    // Initialisation de l'alphabet
    automate.alphabet = '\0';

    // Initialisation de l'état initial de l'automate
    automate.s = 0;
    
    // Initialisation des états finaux de l'automate
    automate.f_size = 0;
    automate.f = NULL;

    // Initialisation des transitions de l'automate
    automate.delta_size = 0;
    automate.delta = NULL;

    return automate;
}

/**
 * Cette fonction permet de renvoyer un automate standard
 * reconnaissant le langage composé du seul mot vide.
 * 
 * @return un AFN reconnaisant le langage vide
 **/ 
AFN automate_mot_vide() {
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

/**
 * Cette fonction permet de renvoyer un automate standard
 * reconnaissant la langage de la réunion de 2 automates.
 * 
 * @param automate1: Le premier automate reconnaissant L1
 * @param automate2: Le deuxiéme automate reconnaissant L2
 * 
 * @return Un automate AFN reconnaissance L1 U L2
 **/ 
AFN automate_reunion(AFN automate1, AFN automate2) {
    AFN automate;
    size_t i;

    // Etats
    automate.q_size = automate1.q_size + automate2.q_size + 1;
    automate.q = (uint*) malloc(sizeof(uint) * automate.q_size);
    *(automate.q) = 0;

    uint added1 = 0, added2 = 0;
    uint ignored1 = 0, ignored2 = 0;
    for (i = 0; i < automate1.q_size; i++) {
        if (*(automate1.q+i) != automate1.s)
            *(automate.q+(++added1)) = *(automate1.q+i);
        else 
            ignored1++;
    }

    automate.q_size -= ignored1;
    automate.q = (uint*) realloc(automate.q, sizeof(uint) * automate.q_size);
    for (i = 0; i < automate2.q_size; i++) {
        if (*(automate2.q+i) != automate2.s)
            *(automate.q+(++added2)+added1) = *(automate2.q+i);
        else
            ignored2++;
    }
    automate.q_size -= ignored2;
    automate.q = (uint*) realloc(automate.q, sizeof(uint) * automate.q_size);
    
    // Etat initial
    automate.s = 0;

    // Etats accepteurs   
    automate.f_size = automate1.f_size + automate2.f_size;
    // Check si s1 ou s2 est etat accepteur!!
    bool is_final = FALSE;

    for (i = 0; !is_final || i < automate1.f_size; i++) {
        if (*(automate1.f+i) == automate1.s)
            is_final = TRUE;
    }
    for (i = 0; !is_final || i < automate2.f_size; i++) {
        if (*(automate2.f+i) == automate2.s)
            is_final = TRUE;
    }
    if (is_final)
        automate.f_size++;
    
    automate.f = (uint*) malloc(sizeof(uint) * automate.f_size);
    if (is_final) 
        *(automate.f) = 0;

    // L'Union de F1 et F2 en respectant les contraintes
    added1 = 0; added2 = 0;
    ignored1 = 0; ignored2 = 0;
    for (i = 0; i < automate1.f_size; i++) {
        if (*(automate1.f+i) != automate1.s)
            *(automate.f+(++added1)) = *(automate1.f+i);
        else 
            ignored1++;
    }

    automate.f_size -= ignored1;
    automate.f = (uint*) realloc(automate.f, sizeof(uint) * automate.f_size);
    for (i = 0; i < automate2.f_size; i++) {
        if (*(automate2.f+i) != automate2.s)
            *(automate.f+(++added2)+added1) = *(automate2.f+i);
        else
            ignored2++;
    }
    automate.f_size -= ignored2;
    automate.f = (uint*) realloc(automate.f, sizeof(uint) * automate.f_size);

    // Transitions
    automate.delta_size = automate1.delta_size + automate2.delta_size;
    automate.delta = (Transition*) malloc(sizeof(Transition) * automate.delta_size);
    for (i = 0; i < automate.delta_size; i++) {
        if (i < automate1.delta_size)
            *(automate.delta+i) = *(automate1.delta+i);
        else
            *(automate.delta+i) = *(automate2.delta+i-automate1.delta_size);
    }

    return automate;
}

/**
 * Cette fonction permet d'afficher le quintuplet d'un automate.
 * 
 * @param automate: L'automate AFN a afficher.
 **/ 
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
