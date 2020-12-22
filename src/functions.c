#include <stdio.h>
#include <stdlib.h>

#include "extra_func.h"
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
    automate.q_size = 0;
    automate.q = NULL;

    // Initialisation de l'état initial de l'automate
    automate.s = NULL;
    
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
 * reconnaissant le langage composé d'un seul mot vide.
 * 
 * @return un AFN reconnaisant le mot vide
 **/ 
AFN automate_mot_vide() {
    AFN automate;

    // La création de l'état initial
    Etat* etat = create_state();

    // Initialisation de l'état initial de l'automate
    automate.s = etat;

    // Initialisation des états de l'automate
    automate.q_size = 1;
    automate.q = (Etat**) malloc(sizeof(Etat*));
    *(automate.q) = etat;

    // Initialisation des états finaux de l'automate
    automate.f_size = 1;
    automate.f = (Etat**) malloc(sizeof(Etat*));
    *(automate.f) = etat;

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

    // La création de l'état initial (s)
    Etat* etat = create_state();

    // La création de l'état destinataire
    Etat* etat_dest = create_state();
    
    // Initialisation de l'état initial de l'automate
    automate.s = etat;

    // Initialisation des états de l'automate
    automate.q_size = 2;
    automate.q = (Etat**) malloc(sizeof(Etat*) * 2);
    *(automate.q) = etat; *(automate.q+1) = etat_dest;

    // Initialisation des états finaux de l'automate
    automate.f_size = 1;
    automate.f = (Etat**) malloc(sizeof(Etat*));
    *(automate.f) = etat_dest;

    // Initialisation des transitions de l'automate
    automate.delta_size = 1;
    automate.delta = (Transition*) malloc(sizeof(Transition));
    (automate.delta)->e1 = etat;
    (automate.delta)->e2 = etat_dest;
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

    if (automate1.q == NULL)
        return automate2;
    else if (automate2.q == NULL)
        return automate1;

    // La création de l'état initial
    Etat* etat = create_state();

    // Initialisation de l'état initial de l'automate
    automate.s = etat;

    // L'ensemble des états (Q)
    automate.q = (Etat**) malloc(sizeof(Etat*));
    automate.q_size = 1;
    *(automate.q) = etat;
    append_Q_to_AFN(&automate, automate1, FALSE);
    append_Q_to_AFN(&automate, automate2, FALSE);
    
    // L'ensemble des états accepteurs (F)
    automate.f = NULL;
    automate.f_size = 0;
    if (is_state_final(automate1, automate1.s) == TRUE || is_state_final(automate2, automate2.s) == TRUE) {
        automate.f_size = 1;
        *(automate.f) = automate.s;
        append_F_to_AFN(&automate, automate1, FALSE);
        append_F_to_AFN(&automate, automate2, FALSE);
    } else {
        append_F_to_AFN(&automate, automate1, TRUE);
        append_F_to_AFN(&automate, automate2, TRUE);
    }
    
    // Transitions (delta)
    automate.delta = NULL;
    automate.delta_size = 0;
    append_DELTA_to_AFN(&automate, automate1);
    append_DELTA_to_AFN(&automate, automate2);

    return automate;
}

/**
 * Cette fonction permet de renvoyer un automate standard
 * reconnaissant la langage de la concaténation de 2 automates.
 * 
 * @param automate1: Le premier automate reconnaissant L1
 * @param automate2: Le deuxiéme automate reconnaissant L2
 * 
 * @return Un automate AFN reconnaissance L1 . L2
 **/ 
AFN automate_concat(AFN automate1, AFN automate2) {
    AFN automate;

    if (automate1.q == NULL)
        return automate2;
    else if (automate2.q == NULL)
        return automate1;

    // Initialisation de l'état initial de l'automate
    automate.s = automate1.s;

    // L'ensemble des états (Q)
    automate.q = NULL;
    automate.q_size = 0;
    append_Q_to_AFN(&automate, automate1, TRUE);
    append_Q_to_AFN(&automate, automate2, FALSE);
    
    // L'ensemble des états accepteurs (F)
    automate.f = NULL;
    automate.f_size = 0;
    if (is_state_final(automate2, automate2.s) == FALSE) {
        automate.f = automate2.f;
        automate.f_size = automate2.f_size;
    } else {
        append_F_to_AFN(&automate, automate1, TRUE);
        append_F_to_AFN(&automate, automate2, FALSE);
    }
    
    // Transitions (delta)
    automate.delta = NULL;
    automate.delta_size = 0;    

    for (size_t i = 0; i < automate1.delta_size; i++) {
        Transition t;
        t.e1 = (automate1.delta+i)->e1;
        t.e2 = (automate1.delta+i)->e2;
        t.alphabet = (automate1.delta+i)->alphabet;
        automate.delta = (Transition*) realloc(automate.delta, sizeof(Transition) * (automate.delta_size + 1));
        *(automate.delta+automate.delta_size) = t;
        automate.delta_size += 1;
    }

    // Copie de toutes les transitions de delta2 qui ne sortent pas de l'état initial 
    for (size_t i = 0; i < automate2.delta_size; i++) {
        Transition t;
        t.e1 = (automate2.delta+i)->e1;
        t.e2 = (automate2.delta+i)->e2;
        t.alphabet = (automate2.delta+i)->alphabet;
        if (t.e1 != automate2.s) { // si l'état p n'est pas s
            automate.delta = (Transition*) realloc(automate.delta, sizeof(Transition) * (automate.delta_size + 1));
            *(automate.delta+automate.delta_size) = t;
            automate.delta_size += 1;
        }
    }

    //
    for (size_t i = 0; i < automate1.f_size; i++) {
        Etat* f1 = *(automate1.f+i);
        for (size_t j = 0; j < automate2.delta_size; j++) {
            Transition t;
            t.e1 = (automate2.delta+j)->e1;
            t.e2 = (automate2.delta+j)->e2;
            t.alphabet = (automate2.delta+j)->alphabet;
            if (t.e1 == automate2.s) { 
                t.e1 = f1;
                automate.delta = (Transition*) realloc(automate.delta, sizeof(Transition) * (automate.delta_size + 1));
                *(automate.delta+automate.delta_size) = t;
                automate.delta_size += 1;
            }
        }
    }

    return automate;
}

AFN automate_kleene(AFN automate_src) {
    AFN automate;

    // L'ensemble des états de l'automate
    automate.q = NULL;
    automate.q_size = 0;
    append_Q_to_AFN(&automate, automate_src, TRUE);

    // L'état initial
    automate.s = automate_src.s;

    // L'ensemble des états accepteurs de l'automate
    automate.f = (Etat**) malloc(sizeof(Etat*));
    automate.f_size = 1;
    *(automate.f) = automate.s;
    append_F_to_AFN(&automate, automate_src, FALSE);

    // L'ensemble des transitions de l'automate
    automate.delta = NULL;
    automate.delta_size = 0;
    // copie de delta de l'automate source (automate_src) dans delta de l'automate destinataire (automate)
    for (size_t j = 0; j < automate_src.delta_size; j++) {
        Transition t;
        t.e1 = (automate_src.delta+j)->e1;
        t.e2 = (automate_src.delta+j)->e2;
        t.alphabet = (automate_src.delta+j)->alphabet;
        automate.delta = (Transition*) realloc(automate.delta, sizeof(Transition) * (automate.delta_size + 1));
        *(automate.delta+automate.delta_size) = t;
        automate.delta_size += 1;
    }
    // 
    for (size_t i = 0; i < automate_src.f_size; i++) {
        Etat* f1 = *(automate_src.f+i);
        for (size_t j = 0; j < automate_src.delta_size; j++) {
            Transition t;
            t.e1 = (automate_src.delta+j)->e1;
            t.e2 = (automate_src.delta+j)->e2;
            t.alphabet = (automate_src.delta+j)->alphabet;
            if (t.e1 == automate_src.s) { 
                t.e1 = f1;
                automate.delta = (Transition*) realloc(automate.delta, sizeof(Transition) * (automate.delta_size + 1));
                *(automate.delta+automate.delta_size) = t;
                automate.delta_size += 1;
            }
        }
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
    for (i = 0; i < automate.q_size; i++) {
        Etat* e = *(automate.q+i);
        printf("%p, ", e);
    }
    printf("}\n");

    printf("s = %p\n", automate.s);

    printf("F = {");
    for (i = 0; i < automate.f_size; i++) {
        Etat* e = *(automate.f+i);
        printf("%p, ", e);
    }
    printf("}\n");

    printf("delta = {");
    for (i = 0; i < automate.delta_size; i++) {
        Etat* e1 = (automate.delta+i)->e1;
        Etat* e2 = (automate.delta+i)->e2;
        char alpha = (automate.delta+i)->alphabet;
        
        printf("(%p, %c, %p), ", e1, alpha, e2);
    }
    printf("}\n\n");
}
