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
    automate.delta = (AFNTransition*) malloc(sizeof(AFNTransition));
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
        automate.f = (Etat**) malloc(sizeof(Etat*));
        automate.f[0] = automate.s;
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
        AFNTransition t;
        t.e1 = (automate1.delta+i)->e1;
        t.e2 = (automate1.delta+i)->e2;
        t.alphabet = (automate1.delta+i)->alphabet;
        automate.delta = (AFNTransition*) realloc(automate.delta, sizeof(AFNTransition) * (automate.delta_size + 1));
        *(automate.delta+automate.delta_size) = t;
        automate.delta_size += 1;
    }

    // Copie de toutes les transitions de delta2 qui ne sortent pas de l'état initial 
    for (size_t i = 0; i < automate2.delta_size; i++) {
        AFNTransition t;
        t.e1 = (automate2.delta+i)->e1;
        t.e2 = (automate2.delta+i)->e2;
        t.alphabet = (automate2.delta+i)->alphabet;
        if (t.e1 != automate2.s) { // si l'état p n'est pas s
            automate.delta = (AFNTransition*) realloc(automate.delta, sizeof(AFNTransition) * (automate.delta_size + 1));
            *(automate.delta+automate.delta_size) = t;
            automate.delta_size += 1;
        }
    }

    //
    for (size_t i = 0; i < automate1.f_size; i++) {
        Etat* f1 = *(automate1.f+i);
        for (size_t j = 0; j < automate2.delta_size; j++) {
            AFNTransition t;
            t.e1 = (automate2.delta+j)->e1;
            t.e2 = (automate2.delta+j)->e2;
            t.alphabet = (automate2.delta+j)->alphabet;
            if (t.e1 == automate2.s) { 
                t.e1 = f1;
                automate.delta = (AFNTransition*) realloc(automate.delta, sizeof(AFNTransition) * (automate.delta_size + 1));
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
        AFNTransition t;
        t.e1 = (automate_src.delta+j)->e1;
        t.e2 = (automate_src.delta+j)->e2;
        t.alphabet = (automate_src.delta+j)->alphabet;
        automate.delta = (AFNTransition*) realloc(automate.delta, sizeof(AFNTransition) * (automate.delta_size + 1));
        *(automate.delta+automate.delta_size) = t;
        automate.delta_size += 1;
    }
    // 
    for (size_t i = 0; i < automate_src.f_size; i++) {
        Etat* f1 = *(automate_src.f+i);
        for (size_t j = 0; j < automate_src.delta_size; j++) {
            AFNTransition t;
            t.e1 = (automate_src.delta+j)->e1;
            t.e2 = (automate_src.delta+j)->e2;
            t.alphabet = (automate_src.delta+j)->alphabet;
            if (t.e1 == automate_src.s) { 
                t.e1 = f1;
                automate.delta = (AFNTransition*) realloc(automate.delta, sizeof(AFNTransition) * (automate.delta_size + 1));
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
    EtatCompose state = automate.s;
    bool compatible = TRUE;
    for (size_t i = 0; (compatible == TRUE) && (i < strlen(str)); i++) {
        char c = *(str+i);
        // trouver toutes les transitions où on peut passer par l'alphabet 'c'
        // si on trouve une transition alors
        //      on passe par cet transition et on mets à jour (state)
        // si on ne trouve pas de transitions alors 
        //      l'automate ne peut pas détecter la chaine de caractère
        AFDTransition* t = NULL;
        AFDTransition** transitions = get_transitions_from_AFD_EtatComp(automate, state);
        AFDTransition* t_pointer = NULL;
        uint t_pos = 0;
        while ( ((t_pointer = transitions[t_pos++]) != NULL) && (t == NULL)){
            // chercher la transition avec l'alphabet 'c'
            if (t_pointer->alphabet == c)
                t = t_pointer;
        }

        if (t == NULL) {
            printf("AUTOMATE NON COMPATIBLE!\n");
            printf("> pas de transition trouvee pour le caractere!\n");
            printf("\tchaine: '%s'\n", str);
            printf("\tcaractere: '%c'\n", c);
            printf("\tpos: %ld\n", i);
            
            compatible = FALSE;
        } else {
            state = t->e2;
        }
    }

    if (compatible == TRUE) {
        printf("AUTOMATE COMPATIBLE\n");
    }
}

AFD determiniser_AFN(AFN automate_src) {
    AFD automate;

    // L'état initial
    automate.s = *compose_state(automate_src.s);
    #if DEBUG==1
    printf("automate.s index : %d\n", automate.s._index);
    #endif
    
    // L'ensemble des états Q
    automate.q_size = 1;
    automate.q = (EtatCompose*) malloc(sizeof(EtatCompose));
    *(automate.q) = automate.s;

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
            etats_composes[j]._index = rand();
            etats_composes[j]._etats = NULL;
            etats_composes[j]._size = 0;
        }
        uint etats_composes_size[ASCII_LENGTH];
        for (size_t j = 0; j < ASCII_LENGTH; j++)
            etats_composes_size[j] = 0;

        for (size_t j = 0; j < etatcomp._size; j++) {
            Etat* state = *(etatcomp._etats+j);

            // list of pointers to transitions
            AFNTransition** transitions = get_transitions_from_AFN(automate_src, state);
            
            size_t pt = 0;
            AFNTransition *t = NULL;

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
                    automate.f[automate.f_size]._index = etatcomp._index;
                    automate.f[automate.f_size]._etats = etatcomp._etats;
                    automate.f[automate.f_size]._size = etatcomp._size;
                    automate.f_size++;
                }
            }
        }
    }

    return automate;
}

AFD minimiser_AFD(AFD automate_src) {
    AFD automate;
    
    // Liste des ensembles des états (séparés)
    EtatCompose** groupes = NULL;
    // Taille de la liste
    uint taille_groupes = 2;
    // Liste des tailles des ensembles
    uint* taille_listes = NULL;
    // Initialisation des deux premiers ensembles
    groupes = (EtatCompose**) malloc(sizeof(EtatCompose*) * taille_groupes);
    groupes[0] = (EtatCompose*) malloc(sizeof(EtatCompose));
    groupes[1] = (EtatCompose*) malloc(sizeof(EtatCompose));
    taille_listes = (uint*) malloc(sizeof(uint) * 2);
    taille_listes[0] = 0;
    taille_listes[1] = 0;

    // Etape 0 (séparation des états accepteurs des autres)
    /**
     * Pour chaque étatcomposé dans Q de l'automate, on vérifie:
     *      si l'étatcomp est accepteur alors 
     *          on l'ajoute dans liste[0] (etatcompose*)
     *      sinon
     *          on l'ajoute dans liste[1]
     **/ 
    #if DEBUG==1
    printf("P0: séparation entre les états accepteurs et non accepteurs\n");
    #endif
    for (size_t i = 0; i < automate_src.q_size; i++) {
        EtatCompose e = automate_src.q[i];
        // vérification si 'e' est un etat accepteur ou pas
        if (existe_etatcompose_AFD_F(automate_src, e) == TRUE) {
            // e est un état accepteur
            groupes[1] = (EtatCompose*) realloc(groupes[1], sizeof(EtatCompose) * (taille_listes[1] + 1));
            groupes[1][taille_listes[1]] = e;
            taille_listes[1]++;
        } else {
            // e n'est pas un état accepteur
            groupes[0] = (EtatCompose*) realloc(groupes[0], sizeof(EtatCompose) * (taille_listes[0] + 1));
            groupes[0][taille_listes[0]] = e;
            taille_listes[0]++;
        }
    }

    // Etapes k (séparation des états)
    #if DEBUG==1
    printf("Pk: séparation des groupes entre eux\n");
    #endif
    for (size_t i = 0; i < taille_groupes; i++) {
        EtatCompose* groupe = groupes[i];
        uint taille_groupe = taille_listes[i];
        if (taille_groupe > 1) {
            for (size_t j = 0; j < taille_groupe; j++) {
                EtatCompose etat_compose = groupe[j];
                LEtatCompose destinations = get_destinations_from_EtatCompose_AFD(automate_src, etat_compose);
                for (size_t y = 0; y < destinations.taille; y++) {
                    EtatCompose dest_state = destinations.etats[y];
                    /**
                     * Si (dest_state) est inclut dans le groupe i, on ne fait rien
                     * Sinon, on sépare le group i en prenant (dest_state) et l'isolant dans un autre groupe
                     **/ 
                    // 1ére étape: vérification si (dest_state) est inclut ou pas dans le groupe i
                    if (est_inclut_dans_groupe(dest_state, groupe, taille_groupe) == FALSE) {
                        // 2éme étape: Si oui, on crée un autre groupe et on l'ajoute dans la liste
                        EtatCompose* new_group = (EtatCompose*) malloc(sizeof(EtatCompose));
                        new_group[0] = etat_compose;
                        groupes[taille_groupes] = new_group;
                        taille_listes = (uint*) realloc(taille_listes, sizeof(uint) * (taille_groupes + 1));
                        taille_listes[taille_groupes++] = 1;
                        #if DEBUG==1
                        printf("\tCreated a new group for state %d\n", etat_compose._index);
                        #endif
                        // 3éme étape: Et, on retire (etat) du groupe i
                        groupe = retirer_etat_groupe(etat_compose, groupe, &taille_groupe);
                    }
                }
            }
        }
    }
    #if DEBUG == 1
    printf("\nThere are %d groupes\n", taille_groupes);
    for (size_t i = 0; i < taille_groupes; i++) {
        EtatCompose* groupe = groupes[i];
        uint taille_groupe = taille_listes[i];
        printf("GROUP %ld\n", i);
        for (size_t j = 0; j < taille_groupe; j++) {
            EtatCompose etat_compose = groupe[j];
            printf("\t%d\n", etat_compose._index);
        }
        printf("-----------------\n");
    }
    #endif

    // L'état initial et les ensembles des états & des états finaux & des transitions
    automate.q = (EtatCompose*) malloc(sizeof(EtatCompose) * taille_groupes);
    automate.q_size = 0;
    automate.f = NULL;
    automate.f_size = 0;
    automate.sigma = NULL;
    automate.sigma_size = 0;
    for (size_t i = 0; i < taille_groupes; i++) {
        EtatCompose* groupe = groupes[i];
        uint taille_groupe = taille_listes[i];
        
        // make a new state composed of all the states in the group
        EtatCompose new_state = compose_state_from_group(groupe, taille_groupe);

        // if the initial state is in the group then make the group the initial state
        if (est_inclut_dans_groupe(automate_src.s, groupe, taille_groupe) == TRUE) {
            automate.s = new_state;
        }

        // add new_state in Q
        #if DEBUG==1
        printf("ADDING composed state to Q [");
        for (size_t j = 0; j < new_state._size; j++) {
            printf("%d, ", new_state._etats[j]->_index);
        }
        printf("]\n\n");
        #endif
        automate.q[automate.q_size++] = new_state;
        
        // check if in the new_state has a final state in it
        if (has_final_state(new_state, automate_src) == TRUE) {
            // add new_state in F
            automate.f = (EtatCompose*) realloc(automate.f, sizeof(EtatCompose) * (automate.f_size  + 1));
            automate.f[automate.f_size++] = new_state;
        }
    }

    // transitions
    for (size_t i = 0; i < automate.q_size; i++) {
        EtatCompose ec = automate.q[i];
        for (size_t j = 0; j < ec._size; j++) {
            #if DEBUG==1
            printf("LOOKING FOR TRANSITIONS OF %d\n", ec._etats[j]->_index);
            #endif
            AFDTransition** liste_transitions = get_transitions_from_AFD(automate_src, ec._etats[j]);
            AFDTransition* current_t = NULL;
            uint current_p = 0;
            while ((current_t = liste_transitions[current_p++]) != NULL)
            {
                AFDTransition new_t;
                new_t.alphabet = current_t->alphabet;
                new_t.e1 = ec;
                new_t.e2 = *get_group(current_t->e2, groupes, taille_groupes, taille_listes);
                #if DEBUG==1
                printf("\n");
                printf("adding (%d, %c, %d)\n", ec._index, current_t->alphabet, current_t->e2._index);
                #endif
                // add new_t to sigma
                automate.sigma = (AFDTransition*) realloc(automate.sigma, sizeof(AFDTransition) * (automate.sigma_size + 1));
                automate.sigma[automate.sigma_size++] = new_t;
            }
        }
    }

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
        printf("%d, ", e->_index);
    }
    printf("}\n");

    printf("s = %p\n", automate.s);

    printf("F = {");
    for (i = 0; i < automate.f_size; i++) {
        Etat* e = *(automate.f+i);
        printf("%d, ", e->_index);
    }
    printf("}\n");

    printf("delta = {");
    for (i = 0; i < automate.delta_size; i++) {
        Etat* e1 = (automate.delta+i)->e1;
        Etat* e2 = (automate.delta+i)->e2;
        char alpha = (automate.delta+i)->alphabet;
        
        printf("(%d, %c, %d), ", e1->_index, alpha, e2->_index);
    }
    printf("}\n\n");
}

void afficherAFD(AFD automate) {
    size_t i;
    printf("Q' = {");
    for (size_t i = 0; i < automate.q_size; i++) {
        EtatCompose etatcomp = automate.q[i];
        printf("%d, ", etatcomp._index);
    }
    printf("}\n");

    printf("s = %d\n", automate.s._index);

    printf("F' = {");
    for (size_t i = 0; i < automate.f_size; i++) {
        EtatCompose etatcomp = automate.f[i];
        printf("%d, ", etatcomp._index);
    }
    printf("}\n");

    printf("delta = {");
    for (i = 0; i < automate.sigma_size; i++) {
        EtatCompose e1 = (automate.sigma+i)->e1;
        EtatCompose e2 = (automate.sigma+i)->e2;
        char alpha = (automate.sigma+i)->alphabet;
        
        printf("(%d, %c, %d), ", e1._index, alpha, e2._index);
    }
    printf("}\n\n");
}
