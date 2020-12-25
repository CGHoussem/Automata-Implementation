#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extra_func.h"
#include "functions.h"

/**
 * ######################
 *  LES FONCTIONS de AFN
 * ######################
 **/

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
 * ######################
 *  LES FONCTIONS de AFD
 * ######################
 **/

void execute_AFD(AFD automate, char* str) {
    Etat* state = automate.s; 
    for (size_t i = 0; i < strlen(str); i++) {
        char c = *(str+i);
        // trouver toutes les transitions où on peut passer par le alphabet 'c'
        // si on ne trouve pas de transitions alors l'automate ne peut pas déteceter la chaine de caractère
        // TODO: to complete
    }
}

AFD determiniser_AFN(AFN automate_src) {
    AFD automate;

    // L'état initial
    automate.s = automate_src.s;
    
    // L'ensemble des états Q
    automate.q_size = 1;
    automate.q = (EtatCompose*) malloc(sizeof(EtatCompose));
    *(automate.q) = compose_state(automate.s);
    
    // L'ensemble des états F
    automate.f = NULL;
    automate.f_size = 0;

    // L'ensemble des transitions
    automate.sigma = NULL;
    automate.sigma_size = 0;

    for (size_t i = 0; i < automate.q_size; i++) {
        EtatCompose etatcomp = *(automate.q+i);
        #if DEBUG==1
        printf("PROCESSING Q[%ld] = %p\n", i, etatcomp._etats[0]);
        #endif
        EtatCompose etats_composes[ASCII_LENGTH];
        for (size_t j = 0; j < ASCII_LENGTH; j++) {
            etats_composes[j]._etats = NULL;
            etats_composes[j]._size = 0;
        }
        uint etats_composes_size[ASCII_LENGTH];
        for (size_t j = 0; j < ASCII_LENGTH; j++)
            etats_composes_size[j] = 0;

        for (size_t j = 0; j < etatcomp._size; j++) {
            Etat* state = *(etatcomp._etats+j);

            // list of pointers to transitions
            Transition** transitions = get_transitions_from_AFN(automate_src, state);
            
            size_t pt = 0;
            Transition *t = NULL;

            // Composer un état
            while ((t = *(transitions+pt))!= NULL) {
                int alpha = (int) (t->alphabet);
                #if DEBUG==1
                printf("\tTransition FOUND from %p to %p\t(%c)\n", state, t->e2, (char)alpha);
                #endif
                // compose or append the etatscompose
                etats_composes[alpha]._etats = (Etat**) malloc(sizeof(Etat*) * (etats_composes_size[alpha] + 1));
                etats_composes[alpha]._etats[etats_composes_size[alpha]] = t->e2;
                etats_composes[alpha]._size = etats_composes_size[alpha] + 1;
                etats_composes_size[alpha]++;

                // increment for loop
                pt += 1;
            }
            #if DEBUG==1
            if (pt == 0) {
                printf("\tNo transitions FOUND from %p\n", state);
            }
            printf("\n");
            #endif
        }

        // Ajout de l'état composé s'il n'existe pas
        for (size_t alpha = 0; alpha < ASCII_LENGTH; alpha++) {
            if (etats_composes_size[alpha] != 0) {
                EtatCompose new_etatcomp = etats_composes[alpha];    
                #if DEBUG==1             
                printf("Transition to (%c) composed with %d state(s)\n", (char)alpha, new_etatcomp._size);
                #endif
                if (existe_etatcompose_AFD_Q(automate, new_etatcomp) == FALSE) {
                    // add new_etatcomp to automate.q
                    #if DEBUG==1
                    printf("\tAJOUT de (%p) dans Q[%d]\n", new_etatcomp._etats[0], automate.q_size);
                    #endif
                    automate.q = (EtatCompose*) realloc(automate.q, sizeof(EtatCompose) * (automate.q_size + 1));
                    *(automate.q+automate.q_size) = new_etatcomp;
                    automate.q_size++;

                    // create a transition from 'etatcomp' to 'new_etatcomp' by 'alpha'
                    AFDTransition new_t;
                    new_t.e1 = etatcomp;
                    new_t.e2 = new_etatcomp;
                    new_t.alphabet = (char)alpha;
                    #if DEBUG==1
                    printf("\tAJOUT de ");
                    printf("(");
                    if (new_t.e1._size == 1)
                        printf("%p", new_t.e1._etats[0]);
                    else {
                        printf("{");
                        for (size_t j = 0; j < new_t.e1._size; j++) {
                            printf("%p, ", new_t.e1._etats[j]);
                        }
                        printf("}, ");
                    }
                    printf(", %c, ", new_t.alphabet);
                    
                    if (new_t.e2._size == 1)
                        printf("%p", new_t.e2._etats[0]);
                    else {
                        printf("{");
                        for (size_t j = 0; j < new_t.e2._size; j++) {
                            printf("%p, ", new_t.e2._etats[j]);
                        }
                        printf("}, ");
                    }
                    printf(") dans SIGMA\n");
                    #endif

                    automate.sigma = (AFDTransition*) realloc(automate.sigma, sizeof(AFDTransition) * (automate.sigma_size + 1));
                    automate.sigma[automate.sigma_size] = new_t;
                    automate.sigma_size++;
                }
                #if DEBUG==1
                printf("\n");
                #endif
            }
        }
    }

    // L'ensemble des états finaux
    automate.f = NULL;
    automate.f_size = 0;
    // find each composed_state in the newly created Q' and set that composed_state as a final state
    for (size_t i = 0; i < automate_src.f_size; i++) {
        Etat* ref_state = *(automate_src.f+i);

        for (size_t j = 0; j < automate.q_size; j++) {
            EtatCompose etatcomp = *(automate.q+j);
            for (size_t y = 0; y < etatcomp._size; y++) {
                if (ref_state->_index == etatcomp._etats[y]->_index) {
                    // add this composed state in F'
                    automate.f = (EtatCompose*) realloc(automate.f, sizeof(EtatCompose) * (automate.f_size + 1));
                    automate.f[automate.f_size]._etats = etatcomp._etats;
                    automate.f[automate.f_size]._size = etatcomp._size;
                    automate.f_size++;
                }
            }
        }
    }

    return automate;
}

AFD minimiser_AFN(AFD automate_src) {
    AFD automate;
    
    // L'etat initial
    automate.s = NULL;

    // L'ensemble des états
    automate.q = NULL;
    automate.q_size = 0;

    // L'ensemble des états finaux
    automate.f = NULL;
    automate.f_size = 0;

    // L'ensemble des transitions
    automate.sigma = NULL;
    automate.sigma_size = 0;

    return automate;
}

/**
 * ######################
 *    AUTRES FONCTIONS 
 * ######################
 **/

/**
 * Cette fonction permet d'afficher le quintuplet d'un automate.
 * 
 * @param automate: L'automate AFN a afficher.
 **/ 
void afficherAFN(AFN automate) {
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

void afficherAFD(AFD automate) {
    size_t i;
    printf("Q' = {");
    for (size_t i = 0; i < automate.q_size; i++) {
        EtatCompose etatcomp = automate.q[i];
        if (etatcomp._size == 1)
            printf("%p, ", *(etatcomp._etats));
        else {
            printf("{");
            for (size_t j = 0; j < etatcomp._size; j++) {
                printf("%p, ", *(etatcomp._etats+j));
            }
            printf("}, ");
        }
    }
    printf("}\n");

    printf("s = %p\n", automate.s);

    printf("F' = {");
    for (size_t i = 0; i < automate.f_size; i++) {
        EtatCompose etatcomp = automate.f[i];
        if (etatcomp._size == 1)
            printf("%p, ", *(etatcomp._etats));
        else {
            printf("{");
            for (size_t j = 0; j < etatcomp._size; j++) {
                printf("%p, ", *(etatcomp._etats+j));
            }
            printf("}, ");
        }
    }
    printf("}\n");

    printf("delta = {");
    for (i = 0; i < automate.sigma_size; i++) {
        EtatCompose e1 = (automate.sigma+i)->e1;
        EtatCompose e2 = (automate.sigma+i)->e2;
        char alpha = (automate.sigma+i)->alphabet;
        
        printf("(");
        if (e1._size == 1)
            printf("%p", e1._etats[0]);
        else {
            printf("{");
            for (size_t j = 0; j < e1._size; j++) {
                printf("%p, ", e1._etats[j]);
            }
            printf("}, ");
        }

        printf(", %c, ", alpha);

        if (e2._size == 1)
            printf("%p, ", e2._etats[0]);
        else {
            printf("{");
            for (size_t j = 0; j < e2._size; j++) {
                printf("%p, ", e2._etats[j]);
            }
            printf("}, ");
        }
        printf("), ");
    }
    printf("}\n\n");
}
