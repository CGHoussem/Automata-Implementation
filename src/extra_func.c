#include <stdio.h>
#include <stdlib.h>

#include "extra_func.h"

void append_Q_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state) {
    if (automate_src.q == NULL)
        return;
    
    // calcul et réallocation de la mémoire de l'ensemble Q

    // faire la concaténation des ensembles Q de l'automate source et l'automate destinataire
    for (size_t i = 0; i < automate_src.q_size; i++) {
        Etat* etat = *(automate_src.q+i);
        if (etat == automate_src.s && include_initial_state == FALSE)
            continue;
        
        automate_dest->q = (Etat**) realloc(automate_dest->q, sizeof(Etat*) * (automate_dest->q_size + 1));
        *(automate_dest->q+automate_dest->q_size) = etat;
        automate_dest->q_size += 1;
    }
}

void append_F_to_AFN(AFN* automate_dest, AFN automate_src, bool include_inital_state) {
    if (automate_src.f == NULL)
        return;

    // faire la concaténation des ensembles F de l'automate source et l'automate destinataire
    for (size_t i = 0; i < automate_src.f_size; i++) {
        Etat* etat = *(automate_src.f+i);
        if (etat == automate_src.s && include_inital_state == FALSE)
            continue;
        // on inclut f{i} dans F
        automate_dest->f = (Etat**) realloc(automate_dest->f, sizeof(Etat*) * (automate_dest->f_size + 1));
        *(automate_dest->f+automate_dest->f_size) = etat;
        automate_dest->f_size += 1;
    }
}

void append_DELTA_to_AFN(AFN* automate_dest, AFN automate_src) {
    if (automate_src.delta == NULL)
        return;

    for (size_t i = 0; i < automate_src.delta_size; i++) {
        AFNTransition t;
        t.e1 = (automate_src.delta+i)->e1;
        t.e2 = (automate_src.delta+i)->e2;
        t.alphabet = (automate_src.delta+i)->alphabet;
        if (t.e1 == automate_src.s) { // si l'état p est l'état initial
            t.e1 = automate_dest->s;
        } 
        automate_dest->delta = (AFNTransition*) realloc(automate_dest->delta, sizeof(AFNTransition) * (automate_dest->delta_size + 1));
        *(automate_dest->delta+automate_dest->delta_size) = t;
        automate_dest->delta_size += 1;
    }
}

AFNTransition** get_transitions_from_AFN(AFN automate_src, Etat* state_src) {
    // Cette liste devra être parcouru jusqu'a l'atteint du pointeur NULL
    AFNTransition** ret = NULL;
    uint ret_size = 0;

    for (size_t i = 0; i < automate_src.delta_size; i++) {
        AFNTransition t = *(automate_src.delta+i);
        if (state_src == t.e1) {
            ret = (AFNTransition**) realloc(ret, sizeof(AFNTransition*) * (ret_size + 1));
            *(ret+ret_size) = automate_src.delta+i;
            ret_size += 1;
        }
    }

    // La fin de la liste
    ret = (AFNTransition**) realloc(ret, sizeof(AFNTransition*) * (ret_size + 1));
    *(ret+ret_size) = NULL;

    return ret;
}

AFNTransition** get_transitions_from_AFN_of_char(AFN automate_src, Etat* state_src, char alpha) {
    // Cette liste devra être parcouru jusqu'a l'atteint du pointeur NULL
    AFNTransition** ret = NULL;
    uint ret_size = 0;

    for (size_t i = 0; i < automate_src.delta_size; i++) {
        AFNTransition t = *(automate_src.delta+i);
        if (state_src == t.e1 && t.alphabet==alpha) {
            ret = (AFNTransition**) realloc(ret, sizeof(AFNTransition*) * (ret_size + 1));
            *(ret+ret_size) = automate_src.delta+i;
            ret_size += 1;
        }
    }

    // La fin de la liste
    if (ret_size > 0) {
        ret = (AFNTransition**) realloc(ret, sizeof(AFNTransition*) * (ret_size + 1));
        *(ret+ret_size) = NULL;
    }

    return ret;
}

AFDTransition** get_transitions_from_AFD_EtatComp(AFD automate_src, EtatCompose state_src) {
    // Cette liste devra être parcouru jusqu'a l'atteint du pointeur NULL
    AFDTransition** ret = NULL;
    uint ret_size = 0;

    for (size_t i = 0; i < automate_src.sigma_size; i++) {
        AFDTransition t = *(automate_src.sigma+i);
        if (state_src._index == t.e1._index) {
            ret = (AFDTransition**) realloc(ret, sizeof(AFDTransition*) * (ret_size + 1));
            *(ret+ret_size) = automate_src.sigma+i;
            ret_size += 1;
        }
    }

    // La fin de la liste
    ret = (AFDTransition**) realloc(ret, sizeof(AFDTransition*) * (ret_size + 1));
    *(ret+ret_size) = NULL;

    return ret;
}

AFDTransition** get_transitions_from_AFD(AFD automate_src, Etat* state_src) {
    // Cette liste devra être parcouru jusqu'a l'atteint du pointeur NULL
    AFDTransition** ret = NULL;
    uint ret_size = 0;

    for (size_t i = 0; i < automate_src.sigma_size; i++) {
        AFDTransition t = automate_src.sigma[i];
        EtatCompose ec = t.e1;
        for (size_t j = 0; j < ec._size; j++) {
            Etat* e = ec._etats[j];
            if (e->_index == state_src->_index) {
                ret = (AFDTransition**) realloc(ret, sizeof(AFDTransition*) * (ret_size + 1));
                ret[ret_size++] = (automate_src.sigma+i);
            }
        }
    }

    // La fin de la liste
    ret = (AFDTransition**) realloc(ret, sizeof(AFDTransition*) * (ret_size + 1));
    *(ret+ret_size) = NULL;

    return ret;
}

LEtatCompose get_destinations_from_EtatCompose_AFD(AFD automate_src, EtatCompose etatcompose_src) {
    LEtatCompose liste;
    liste.etats = NULL;
    liste.taille = 0;

    for (size_t i = 0; i < automate_src.sigma_size; i++) {
        AFDTransition t = automate_src.sigma[i];
        if (t.e1._index == etatcompose_src._index) {
            liste.etats = (EtatCompose*) realloc(liste.etats, sizeof(EtatCompose) * (liste.taille + 1));
            liste.etats[liste.taille++] = t.e2;
        }
    }

    return liste;
}

EtatCompose* get_group(EtatCompose etat, EtatCompose** groupes, uint taille_groupes, uint* taille_listes) {
    for (size_t i = 0; i < taille_groupes; i++) {
        EtatCompose* group = groupes[i];
        uint taille = taille_listes[i];
        #if DEBUG==1
        printf("search if %d in %d\n", etat._index, group->_index);
        #endif
        if (est_inclut_dans_groupe(etat, group, taille) == TRUE) {
            #if DEBUG==1
            printf("\tfound in %d\n", group->_index);
            #endif
            return group;
        }
    }
    return NULL;    
}

Etat* create_state() {
    Etat* state = (Etat*) malloc(sizeof(Etat));
    state->_index = rand();

    return state;
}

bool is_state_final(AFN automate_src, Etat* etat) {
    for (size_t i = 0; i < automate_src.f_size; i++) {
        Etat* state = *(automate_src.f+i);
        if (etat == state)
            return TRUE;
    }
    return FALSE;
}

bool existe_etatcompose_AFD_Q(AFD automate_src, EtatCompose etats_src) {
    if (automate_src.q_size <= 0)
        return FALSE;
    
    for (size_t i = 0; i < automate_src.q_size; i++) {
        EtatCompose* etatcomp = automate_src.q+i;
        
        // si les tailles ne sont pas égaux, c'est même pas la peine de vérifier le reste
        if (etatcomp->_size != etats_src._size)
            continue;
        
        bool ret = TRUE;
        for (size_t j = 0; j < etatcomp->_size; j++) {
            if (etatcomp->_etats[j]->_index != etats_src._etats[j]->_index) {
                ret = FALSE;
                break;
            }
        }

        if (ret == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}

bool existe_etatcompose_AFD_F(AFD automate_src, EtatCompose etatcompose_src) {
    if (automate_src.f_size == 0)
        return FALSE;
    
    for (size_t i = 0; i < automate_src.f_size; i++) {
        EtatCompose etatcomp = *(automate_src.f+i);
        if (etatcomp._index == etatcompose_src._index)
            return TRUE;
    }

    return FALSE;
}

bool existe_etatcompose_liste(EtatCompose etat, EtatCompose* liste, uint taille) {
    if (taille == 0)
        return FALSE;

    for (size_t i = 0; i < taille; i++) {
        EtatCompose ec = liste[i];
        
        // si les tailles ne sont pas égaux, c'est même pas la peine de vérifier le reste
        if (ec._size != etat._size)
            continue;

        bool ret = TRUE;
        for (size_t j = 0; j < ec._size; j++) {
            if (ec._etats[j]->_index != etat._etats[j]->_index) {
                ret = FALSE;
                break;
            }
        }

        if (ret == TRUE)
            return TRUE;
    }

    return FALSE;
}

EtatCompose* get_equivalent_etatcompose_liste(EtatCompose etat, EtatCompose* liste, uint taille) {
    if (taille == 0)
        return NULL;

    for (size_t i = 0; i < taille; i++) {
        EtatCompose* ec = (liste+i);
        
        // si les tailles ne sont pas égaux, c'est même pas la peine de vérifier le reste
        if (ec->_size != etat._size)
            continue;

        bool ret = TRUE;
        for (size_t j = 0; j < ec->_size; j++) {
            if (ec->_etats[j]->_index != etat._etats[j]->_index) {
                ret = FALSE;
                break;
            }
        }

        if (ret == TRUE)
            return ec;
    }

    return NULL;
}

bool est_inclut_dans_groupe(EtatCompose etat, EtatCompose* groupe, uint taille) {
    for (size_t i = 0; i < taille; i++) {
        EtatCompose e = groupe[i];
        if (e._index == etat._index)
            return TRUE;
    }

    return FALSE;
}

bool has_final_state(EtatCompose etat, AFD automate_src) {
    for (size_t i = 0; i < automate_src.f_size; i++) {
        EtatCompose ec_src = automate_src.f[i];
        for (size_t j = 0; j < ec_src._size; j++) {
            Etat* e_src = ec_src._etats[j];
            for (size_t k = 0; k < etat._size; k++) {
                Etat* e = etat._etats[k];
                if (e->_index == e_src->_index)
                    return TRUE;
            }
        }
    }

    return FALSE;
}

EtatCompose* compose_state(Etat* state) {
    if (state == NULL)
        return NULL;
    EtatCompose* etat_s = (EtatCompose*) malloc(sizeof(EtatCompose));
    etat_s->_index = rand();
    etat_s->_etats = (Etat**) malloc(sizeof(Etat*));
    etat_s->_etats[0] = state;
    etat_s->_size = 1;
    return etat_s;
}

EtatCompose compose_state_from_group(EtatCompose* liste, uint taille) {
    EtatCompose etat;

    etat._index = rand();
    etat._size = 0;
    etat._etats = NULL;

    #if DEBUG==1
    printf("COMPOSING a state from group %d [", liste->_index);
    #endif
    for (size_t i = 0; i < taille; i++) {
        EtatCompose ec = liste[i];
        etat._etats = (Etat**) realloc(etat._etats, sizeof(Etat*) * ec._size);
        #if DEBUG==1
        printf("%d, ", ec._index);
        #endif

        for (size_t j = 0; j < ec._size; j++) {
            Etat* e = ec._etats[j];
            etat._etats[etat._size++] = e;
        }
    }
    #if DEBUG==1
    printf("]\n");
    #endif

    return etat;
}

EtatCompose* retirer_etat_groupe(EtatCompose etat, EtatCompose* groupe, uint* taille) {
    EtatCompose* altered_group = groupe;
    int pos = -1;
    
    // Search the position of the state to be delete
    for (size_t i = 0; (pos == -1) && (i < *(taille)); i++) {
        EtatCompose e = groupe[i];
        if (e._index == etat._index)
            pos = i;
    }

    // Making sure that we found the state to be deleted
    if (pos != -1) {
        // delete the state from the groupe
        for (size_t i = pos; i < *(taille)-1; i++) {
            altered_group[i] = altered_group[i+1];
        }
        *(taille) -= 1;
        #if DEBUG==1
        printf("\tl'état %d a été retirer\n", etat._index);
        #endif
    }

    return altered_group;
}