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
        Transition t;
        t.e1 = (automate_src.delta+i)->e1;
        t.e2 = (automate_src.delta+i)->e2;
        t.alphabet = (automate_src.delta+i)->alphabet;
        if (t.e1 == automate_src.s) { // si l'état p est l'état initial
            t.e1 = automate_dest->s;
        } 
        automate_dest->delta = (Transition*) realloc(automate_dest->delta, sizeof(Transition) * (automate_dest->delta_size + 1));
        *(automate_dest->delta+automate_dest->delta_size) = t;
        automate_dest->delta_size += 1;
    }
}

Transition** get_transitions_from_AFN(AFN automate_src, Etat* state_src) {
    // Cette liste devra être parcouru jusqu'a l'atteint du pointeur NULL
    Transition** ret = NULL;
    uint ret_size = 0;

    for (size_t i = 0; i < automate_src.delta_size; i++) {
        Transition t = *(automate_src.delta+i);
        if (state_src == t.e1) {
            ret = (Transition**) realloc(ret, sizeof(Transition*) * (ret_size + 1));
            *(ret+ret_size) = automate_src.delta+i;
            ret_size += 1;
        }
    }

    // La fin de la liste
    ret = (Transition**) realloc(ret, sizeof(Transition*) * (ret_size + 1));
    *(ret+ret_size) = NULL;

    return ret;
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
        // printf("CHECK %p && %p\n", etats_src._etats, etatcomp->_etats);
        // if (etats_src._etats == etatcomp->_etats) {
        //     printf("FOUND!!!!\n");
        //     return TRUE;
        // }
    }
    return FALSE;
}

EtatCompose compose_state(Etat* state) {
    EtatCompose etat_s;
    etat_s._etats = (Etat**) malloc(sizeof(Etat*));
    *(etat_s._etats) = state;
    etat_s._size = 1;
    return etat_s;
}

void add_to_EtatCompose(EtatCompose* composed_state, Etat* state) {
    composed_state->_etats = (Etat**) malloc(sizeof(Etat*) * (composed_state->_size + 1));
    *(composed_state->_etats+composed_state->_size) = state;
    composed_state->_size += 1;
}
