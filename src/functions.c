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
 * standard reconnaissant un mot d'un caractere donné.
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
    // Copie de toutes les transitions de delta1
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

/**
 * Cette fonction permet de renvoyer un automate standard
 * reconnaissant la représentation kleene étoile de l'automate
 * passé en paramétre.
 * 
 * @param automate_src: L'automate source
 * 
 * @return L'automate reconnaissant la représentation kleene étoile
 **/ 
AFN automate_kleene(AFN automate_src) {
    AFN automate;

    // L'ensemble des états de l'automate
    automate.q = NULL;
    automate.q_size = 0;
    append_Q_to_AFN(&automate, automate_src, TRUE);

    // L'état initial
    automate.s = automate_src.s;

    // L'ensemble d'états accepteurs de l'automate
    automate.f = (Etat**) malloc(sizeof(Etat*));
    automate.f_size = 1;
    *(automate.f) = automate.s;
    append_F_to_AFN(&automate, automate_src, FALSE);

    // L'ensemble de transitions de l'automate
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

/**
 * Cette fonction permet d'exécuter une chaine de caractére sur un automate AFD.
 * 
 * @param automate: L'automate sur lequel la chaine de caractére est executée
 * @param str: La chaine de caractére
 **/ 
void execute_AFD(AFD automate, char* str) {
    printf("L'exécution de l'automate avec la chaine de caractéres: %s\n", str);
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
            printf("> Il n'existe pas de transition pour '%c' a partir de l'etat %d\n", c, state._index);
            printf("\tchaine: '%s'\n", str);
            printf("\tcaractere: '%c'\n", c);
            printf("\tpos de blocage: %ld\n", i+1);
            
            compatible = FALSE;
        } else {
            state = t->e2;
        }
    }
    if (compatible == TRUE) {
        if (existe_etatcompose_AFD_F(automate, state) == TRUE) 
            printf("AUTOMATE COMPATIBLE!\n");
        else {
            printf("AUTOMATE NON COMPATIBLE!\n");
            printf("> Le dernier etat n'est pas un etat accepteur!\n");
            printf("\tchaine: '%s'\n", str);
            printf("\tcaractere: '%c'\n", str[strlen(str)-1]);
            printf("\tpos de blocage: %ld\n", strlen(str));
        }
    }
    printf("\n\n");
}

/**
 * Cette fonction permet de déterminiser un AFN et renvoyer l'AFD résultant
 * 
 * @param automate_src: L'automate fini non déterministe a déterminiser
 * 
 * @return L'automate fini déterministe
 **/ 
AFD determiniser_AFN(AFN automate_src) {
    AFD automate;

    // temp Q
    EtatCompose* temp_q = (EtatCompose*) malloc(sizeof(EtatCompose));
    uint temp_q_size = 1;
    temp_q[0] = *compose_state(automate_src.s);

    // pour chaque état composé dans temp_q
    for (size_t i = 0; i < temp_q_size; i++) {
        EtatCompose qi = temp_q[i];
        // pour chaque état dans qi
        for (size_t j = 0; j < qi._size; j++) {
            Etat* qij = qi._etats[j];
            // pour chaque alphabet dans la langage
            for (size_t c = 0; c < ASCII_LENGTH; c++) {
                // on regarde toutes les transitions vers l'alphabet 'c'
                AFNTransition** transitions = get_transitions_from_AFN_of_char(automate_src, qij, (char)c);
                if (transitions != NULL) {
                    uint length = 0;
                    size_t p = 0;
                    AFNTransition* t = NULL;
                    // calcul combien il existe de transitions de l'état à partir du caractere 'c'
                    while ((t = transitions[p++]) != NULL) length++;
                    #if DEBUG==1
                    printf("FOUND %d transitions for %c\n", length, (char)c);
                    #endif
                    if (length <= 1) {
                        // on vérifie si l'état composé existe déja dans le temp_q
                        EtatCompose ec = *compose_state(transitions[0]->e2);
                        if (existe_etatcompose_liste(ec, temp_q, temp_q_size) == FALSE) {
                            // on ajoute directement l'état à temp_q
                            #if DEBUG==1
                            printf("AJOUT de %d dans Q_temp %d\n", ec._index, temp_q->_index);
                            #endif
                            temp_q = (EtatCompose*) realloc(temp_q, sizeof(EtatCompose) * (temp_q_size + 1));
                            temp_q[temp_q_size++] = ec;
                        }
                    }
                    else {
                        // on compose un seul état de toutes les états destinataires
                        AFNTransition* t = NULL;
                        p = 0;
                        EtatCompose temp;
                        temp._index = rand();
                        temp._etats = (Etat**) malloc(sizeof(Etat*) * length);
                        while ((t = transitions[p++]) != NULL) {
                            temp._etats[temp._size++] = t->e2;
                        }
                        // on vérifie si l'état composé existe déja dans le temp_q
                        if (existe_etatcompose_liste(temp, temp_q, temp_q_size) == FALSE) {
                            // ajoute l'état composé à temp_q
                            #if DEBUG==1
                            printf("AJOUT de %d dans Q_temp %d\n", temp._index, temp_q->_index);
                            #endif
                            temp_q = (EtatCompose*) realloc(temp_q, sizeof(EtatCompose) * (temp_q_size + 1));
                            temp_q[temp_q_size++] = temp;
                        }
                    }
                }
            }
        }
    }

    #if DEBUG==1
    printf("==============TEMP============\n");
    printf("Q temp: [");
    for (size_t i = 0; i < temp_q_size; i++) {
        EtatCompose ec = temp_q[i];
        printf("[%d](", ec._index);
        for (size_t j = 0; j < ec._size; j++) {
            printf("%d, ", ec._etats[j]->_index);
        }
        printf("), ");
    }
    printf("]\n");
    #endif
    
    EtatCompose temp_s = {._etats = NULL, ._size = 0, ._index=0};
    // trouver temp_s
    for (size_t i = 0; i < temp_q_size; i++) {
        EtatCompose ec = temp_q[i];
        if (ec._size == 1) {
            if (ec._etats[0] == automate_src.s) {
                temp_s = ec;
                break;
            }
        }
    }
    // TODO: to look into ('maybe used uninitialized' warning)
    if (temp_s._etats == NULL){
        exit(1);
    }

    #if DEBUG==1
    printf("s temp: [%d](", temp_s._index);
    for (size_t j = 0; j < temp_s._size; j++) {
        printf("%d, ", temp_s._etats[j]->_index);
    }
    printf(")\n");
    #endif

    // trouver f
    EtatCompose* temp_f = NULL;
    uint temp_f_size = 0;
    for (size_t i = 0; i < temp_q_size; i++) {
        EtatCompose ec = temp_q[i];
        bool done = FALSE;
        for (size_t j = 0; (done==FALSE) && (j < ec._size); j++) {
            Etat* e = ec._etats[j];
            if (is_state_final(automate_src, e) == TRUE){
                temp_f = (EtatCompose*) realloc(temp_f, sizeof(EtatCompose) * (temp_f_size + 1));
                temp_f[temp_f_size++] = ec;
                done = TRUE;
            }
        }
    }

    #if DEBUG==1
    printf("F temp: [");
    for (size_t i = 0; i < temp_f_size; i++) {
        EtatCompose ec = temp_f[i];
        printf("[%d](", ec._index);
        for (size_t j = 0; j < ec._size; j++) {
            printf("%d, ", ec._etats[j]->_index);
        }
        printf("), ");
    }
    printf("]\n");
    #endif

    // les transitions
    AFDTransition* temp_sigma = NULL;
    uint temp_sigma_size = 0;
    for (size_t i = 0; i < temp_q_size; i++) {
        EtatCompose ec = temp_q[i];

        // look for all transitions of ec._etats in the AFN
        for (size_t j = 0; j < ec._size; j++) {
            Etat* e1 = ec._etats[j];
            AFNTransition** transitions = get_transitions_from_AFN(automate_src, e1);

            // for each transition, we take E2 and search its equivalent in temp_q and LINK
            AFNTransition* current_t = NULL;
            uint current_p = 0;
            while ((current_t = transitions[current_p++]) != NULL) {
                EtatCompose temp = *compose_state(current_t->e2);
                EtatCompose* eq = get_equivalent_etatcompose_liste(temp, temp_q, temp_q_size);
                if (eq != NULL) {
                    // LINK: make a transition between ec and eq by current_t.alphabet
                    AFDTransition new_t;
                    new_t.e1 = ec;
                    new_t.e2 = *eq;
                    new_t.alphabet = current_t->alphabet;
                    temp_sigma = (AFDTransition*) realloc(temp_sigma, sizeof(AFDTransition) * (temp_sigma_size + 1));
                    temp_sigma[temp_sigma_size] = new_t;
                    temp_sigma_size++;
                }
            }
        }    
    }

    #if DEBUG==1
    printf("sigma = {");
    for (size_t i = 0; i < temp_sigma_size; i++) {
        EtatCompose e1 = (temp_sigma+i)->e1;
        EtatCompose e2 = (temp_sigma+i)->e2;
        char alpha = (temp_sigma+i)->alphabet;
        
        printf("(%d, %c, %d), ", e1._index, alpha, e2._index);
    }
    printf("}\n");
    printf("==============TEMP============\n");
    printf("\n\n");
    #endif

    automate.s = temp_s;
    automate.q = temp_q;
    automate.q_size = temp_q_size;
    automate.f = temp_f;
    automate.f_size = temp_f_size;
    automate.sigma = temp_sigma;
    automate.sigma_size = temp_sigma_size;

    return automate;
}

/**
 * Cette fonction permet de minimiser un automate fini déterministe (AFD)
 * passé en paramétre et renvoyer l'automate résultant.
 * 
 * @param automate_src: L'automate a minimisé
 * 
 * @return L'automate minimisé
 **/ 
AFD minimiser_AFD(AFD automate_src) {
    AFD automate;
    
    #if DEBUG==1
    printf("\n=================TEMP===============\n");
    #endif
    uint temp_sets_size = 0;
    EtatCompose** temp_sets = NULL;
    uint* sets_sizes = NULL;
    // 0 Equivalence: séparation des états accepteurs et non accepteurs
    uint accepteurs_size = 0;
    EtatCompose* accepteurs = NULL;
    uint non_accepteurs_size = 0;
    EtatCompose* non_accepteurs = NULL;
    for (size_t i = 0; i < automate_src.q_size; i++) {
        EtatCompose ec = automate_src.q[i];
        if (existe_etatcompose_AFD_F(automate_src, ec) == TRUE) {
            accepteurs = (EtatCompose*) realloc(accepteurs, sizeof(EtatCompose) * (accepteurs_size+1));
            accepteurs[accepteurs_size++] = ec;
        } else {
            non_accepteurs = (EtatCompose*) realloc(non_accepteurs, sizeof(EtatCompose) * (non_accepteurs_size+1));
            non_accepteurs[non_accepteurs_size++] = ec;
        }
    }
    // ajout des listes d'états accepteur et non accepteurs dans la liste des sets
    if (non_accepteurs_size > 0) {
        temp_sets = (EtatCompose**) malloc(sizeof(EtatCompose*) * (temp_sets_size + 1));
        temp_sets[temp_sets_size] = (EtatCompose*) malloc(sizeof(EtatCompose) * non_accepteurs_size);
        sets_sizes = (uint*) malloc(sizeof(uint) * (temp_sets_size + 1));
        sets_sizes[temp_sets_size] = non_accepteurs_size;
        for (size_t i = 0; i < non_accepteurs_size; i++) {
            temp_sets[temp_sets_size][i] = non_accepteurs[i];
        }
        temp_sets_size++;
    }
    if (accepteurs_size > 0) {
        temp_sets = (EtatCompose**) realloc(temp_sets, sizeof(EtatCompose*) + (temp_sets_size + 1));
        temp_sets[temp_sets_size] = (EtatCompose*) malloc(sizeof(EtatCompose) * accepteurs_size);
        sets_sizes = (uint*) realloc(sets_sizes, sizeof(uint) * (temp_sets_size + 1));
        sets_sizes[temp_sets_size] = accepteurs_size;
        for (size_t i = 0; i < accepteurs_size; i++) {
            temp_sets[temp_sets_size][i] = accepteurs[i];
        }
        temp_sets_size++;
    }
    #if DEBUG==1
    printf("0 équivalence est prêt!\n\tnon accepteurs: %d\n\taccepteurs: %d\n\tIl existe %d set(s)\n", non_accepteurs_size, accepteurs_size, temp_sets_size);
    #endif

    // k Equivalences: ..
    for (size_t i = 0; i < temp_sets_size; i++) {
        EtatCompose* set = temp_sets[i];
        uint set_size = sets_sizes[i];
        LEtatCompose old_lec = {.etats=NULL, .taille=0};
        for (size_t j = 0; j < set_size; j++) {
            EtatCompose ec = set[j];
            LEtatCompose new_lec = get_destinations_from_EtatCompose_AFD(automate_src, ec);
            if (old_lec.taille != 0) {
                // comparing old lec to new lec
                if (is_lec_same(old_lec, new_lec) == FALSE) {
                    // Créer un set contenant l'état ec et on l'ajoute dans la liste des sets
                    EtatCompose* new_set = (EtatCompose*) malloc(sizeof(EtatCompose));
                    new_set[0] = ec;
                    
                    temp_sets = (EtatCompose**) realloc(temp_sets, sizeof(EtatCompose*) * (temp_sets_size + 1));
                    temp_sets[temp_sets_size] = new_set;
                    
                    sets_sizes = (uint*) realloc(sets_sizes, sizeof(uint) * (temp_sets_size + 1));
                    sets_sizes [temp_sets_size++] = 1;
                    
                    // On retire (etat) du set courant
                    retirer_etat_groupe(ec, set, &set_size);
                    temp_sets[i] = (EtatCompose*) realloc(temp_sets[i], sizeof(EtatCompose) * set_size);
                    temp_sets[i] = set;
                    sets_sizes[i] = set_size;;
                }
            }
            old_lec = new_lec;
        }
        #if DEBUG==1
        printf("%ld équivalence est prêt!\n\tIl existe %d set(s)\n", (i+1), temp_sets_size);
        printf("\tTemp Sets: ");
        for (size_t j = 0; j < temp_sets_size; j++) {
            EtatCompose* set = temp_sets[j];
            uint set_size = sets_sizes[j];
            printf("{");
            for (size_t y = 0; y < set_size; y++) {
                EtatCompose ec = set[y];
                printf("%d, ", ec._index);
            }
            printf("}, ");
        }
        printf("\n");
        #endif
    }

    // TEMP Q && TEMP S && TEMP F
    EtatCompose* temp_q = (EtatCompose*) malloc(sizeof(EtatCompose) * temp_sets_size);
    uint temp_q_size = temp_sets_size;
    EtatCompose temp_s = {._size=0, ._etats = NULL, ._index=0};
    EtatCompose* temp_f = NULL;
    uint temp_f_size = 0;
    for (size_t i = 0; i < temp_sets_size; i++) {
        EtatCompose* set = temp_sets[i];
        uint set_size = sets_sizes[i];
        // make a new state composed of all the states in the group
        EtatCompose new_state = compose_state_from_group(set, set_size);
        // adding new_state to temp_q
        temp_q[i] = new_state;
        // if the initial state is in the set then make the set the initial state
        if (est_inclut_dans_groupe(automate_src.s, set, set_size) == TRUE)
            temp_s = new_state;
        // check if in the new_state has a final state in it
        if (has_final_state(new_state, automate_src) == TRUE) {
            // add new_state in F
            temp_f = (EtatCompose*) realloc(temp_f, sizeof(EtatCompose) * (temp_f_size  + 1));
            temp_f[temp_f_size++] = new_state;
        }
    }
    // TEMP SIGMA
    AFDTransition* temp_sigma = NULL;
    uint temp_sigma_size = 0;
    for (size_t i = 0; i < automate_src.sigma_size; i++) {
        AFDTransition transition = automate_src.sigma[i];
        AFDTransition new_transition = {};
        EtatCompose e1 = transition.e1;
        EtatCompose e2 = transition.e2;
        new_transition.alphabet = transition.alphabet;
        // get the equivalent state of e1 in temp_q
        EtatCompose* state1 = NULL;
        for (size_t j = 0; (state1==NULL) && (j < temp_q_size); j++) {
            EtatCompose set = temp_q[j];
            for (size_t y = 0; (state1==NULL) && (y < e1._size); y++) {
                for (size_t k = 0; k < set._size; k++) {
                    if (set._etats[k]->_index == e1._etats[y]->_index) {
                        state1 = &set;
                        break;
                    }
                }
            }
        }
        // get the equivalent state of e2 in temp_q
        EtatCompose* state2 = NULL;
        for (size_t j = 0; (state2==NULL) && (j < temp_q_size); j++) {
            EtatCompose set = temp_q[j];
            for (size_t y = 0; (state2==NULL) && (y < e2._size); y++) {
                for (size_t k = 0; k < set._size; k++) {
                    if (set._etats[k]->_index == e2._etats[y]->_index) {
                        state2 = &set;
                        break;
                    }
                }
            }
        }
        // 
        new_transition.e1 = *state1;
        new_transition.e2 = *state2;
        // check the existance of new_transition in temp_sigma
        if (does_afdtransition_exist(new_transition, temp_sigma, temp_sigma_size) == FALSE) {
            // add new_transition to temp_sigma
            temp_sigma = (AFDTransition*) realloc(temp_sigma, sizeof(AFDTransition) * (temp_sigma_size + 1));
            temp_sigma[temp_sigma_size++] = new_transition;
        }
    }
    #if DEBUG==1
    printf("\n---TEMP_Q---\n\t[");
    for (size_t i = 0; i < temp_q_size; i++) {
        EtatCompose ec = temp_q[i];
        printf("(%d){", ec._index);
        for (size_t j = 0; j < ec._size; j++) {
            printf("%d, ", ec._etats[j]->_index);
        }
        printf("}, ");
    }
    printf("]\n---TEMP_S---\n\t(%d)\n---TEMP_F---\n\t[", temp_s._index);
    for (size_t i = 0; i < temp_q_size; i++) {
        EtatCompose ec = temp_q[i];
        printf("(%d), ", ec._index);
    }
    printf("]\n---TEMP_SIGMA---\n\t[");
    for (size_t i = 0; i < temp_sigma_size; i++) {
        AFDTransition transition = temp_sigma[i];
        printf("(%d, %c, %d), ", transition.e1._index, transition.alphabet, transition.e2._index);
    }
    printf("]\n=================TEMP===============\n\n");
    #endif
    
    automate.q = temp_q;
    automate.q_size = temp_q_size;
    automate.f = temp_f;
    automate.f_size = temp_f_size;
    automate.s = temp_s;
    automate.sigma = temp_sigma;
    automate.sigma_size = temp_sigma_size;

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
        if (i + 1 >= automate.q_size)
            printf("%d", e->_index);
        else
            printf("%d, ", e->_index);
    }
    printf("}\n");

    printf("s = %d\n", automate.s->_index);

    printf("F = {");
    for (i = 0; i < automate.f_size; i++) {
        Etat* e = *(automate.f+i);
        if (i + 1 >= automate.f_size)
            printf("%d", e->_index);
        else
            printf("%d, ", e->_index);
    }
    printf("}\n");

    printf("delta = {\n");
    for (i = 0; i < automate.delta_size; i++) {
        Etat* e1 = (automate.delta+i)->e1;
        Etat* e2 = (automate.delta+i)->e2;
        char alpha = (automate.delta+i)->alphabet;
        
        printf("\t(%d -> %c -> %d)\n", e1->_index, alpha, e2->_index);
    }
    printf("}\n\n");
}

void afficherAFD(AFD automate) {
    size_t i;
    printf("Q' = {");
    for (size_t i = 0; i < automate.q_size; i++) {
        EtatCompose etatcomp = automate.q[i];
        if (i + 1 >= automate.q_size)
            printf("%d", etatcomp._index);
        else
            printf("%d, ", etatcomp._index);
    }
    printf("}\n");

    printf("s = %d\n", automate.s._index);

    printf("F' = {");
    for (size_t i = 0; i < automate.f_size; i++) {
        EtatCompose etatcomp = automate.f[i];
        if (i + 1 >= automate.f_size)
            printf("%d", etatcomp._index);
        else
            printf("%d, ", etatcomp._index);
    }
    printf("}\n");

    printf("sigma = {\n");
    for (i = 0; i < automate.sigma_size; i++) {
        EtatCompose e1 = (automate.sigma+i)->e1;
        EtatCompose e2 = (automate.sigma+i)->e2;
        char alpha = (automate.sigma+i)->alphabet;
        
        printf("\t(%d -> %c -> %d)\n", e1._index, alpha, e2._index);
    }
    printf("}\n\n");
}
