#include <stdio.h>
#include <stdlib.h>

#include "extra_func.h"

/**
 * Cette fonction permet d'ajouter les états de l'ensemble Q d'un automate source 
 * à l'ensemble Q d'un automate destinataire.
 * 
 * @param automate_dest: L'addresse de l'automate destinataire
 * @param automate_src: L'automate source
 * @param include_initial_state: Inclure l'état initial ou pas? 
 **/ 
void append_Q_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state) {
    if (automate_src.q == NULL)
        return;
    
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

/**
 * Cette fonction permet d'ajouter les états de l'ensemble F d'un automate source 
 * à l'ensemble F d'un automate destinataire.
 * 
 * @param automate_dest: L'addresse de l'automate destinataire
 * @param automate_src: L'automate source
 * @param include_initial_state: Inclure l'état initial ou pas? 
 **/ 
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

/**
 * Cette fonction permet d'ajouter la liste de transitions DELTA d'un automate source 
 * à la liste des transitions DELTA d'un automate destinataire.
 * 
 * @param automate_dest: L'addresse de l'automate destinataire
 * @param automate_src: L'automate source
 **/ 
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

/**
 * Cette fonction permet de renvoyer les transitions d'un AFN ayant
 * state_src comme état source des transitions
 * 
 * @param automate_src: L'automate AFN source
 * @param state_src: L'état source
 * 
 * @return La liste de transitions
 **/
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

/**
 * Cette fonction permet de renvoyer les transitions d'un AFN ayant comme 
 * état source 'state_src' et caractère 'alpha'
 * 
 * @param automate_src: L'automate source
 * @param state_src: Le pointeur de l'état source des transitions
 * @param alpha: Le caractére des transitions
 * 
 * @return La liste de transitions
 **/ 
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

/**
 * Cette fonction permet de renvoyer les transitions d'un AFD ayant
 * state_src comme état source des transitions
 * 
 * @param automate_src: L'automate source
 * @param state_src: L'état source
 * 
 * @return Liste des transitions
 **/
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

/**
 * Cette fonction permet de renvoyer une liste d'états composés destinataires
 * d'un état composé et d'un AFD passés en paramétre
 * 
 * @param automate_src: Un automate fini déterministe
 * @param etatcompose_src: L'état composé source
 * 
 * @return Liste d'états composés
 **/ 
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

/**
 * Cette fonction permet de créer un état et retourne son pointeur.
 * 
 * @return Pointeur vers l'état crée.
 **/ 
Etat* create_state() {
    Etat* state = (Etat*) malloc(sizeof(Etat));
    state->_index = rand();

    return state;
}

/**
 * Cette fonction permet de vérifier si un état donné est
 * un état accepteur d'un automate donné ou pas.
 * 
 * @param automate_src: L'automate source
 * @param etat: Le pointeur de l'état à vérifier
 * 
 * @return Si l'état est accepteur ou pas?
 **/ 
bool is_state_final(AFN automate_src, Etat* etat) {
    for (size_t i = 0; i < automate_src.f_size; i++) {
        Etat* state = *(automate_src.f+i);
        if (etat == state)
            return TRUE;
    }
    return FALSE;
}

/**
 * Cette fonction permet de vérifier et renvoyer si un état composé passé
 * en paramétre existe dans l'ensemble d'états d'un AFD ou pas
 * 
 * @param automate_src: L'automate source (AFD)
 * @param etats_src: L'état composé à vérifier
 * 
 * @return Une valeur booléene qui détermine si l'état existe ou pas
 **/ 
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

/**
 * Cette fonction permet de renvoyer si un état passé en paramétre 
 * existe dans l'ensemble d'états accepteurs d'un automate passé en paramétre ou pas
 * 
 * @param automate_src: L'automate source
 * @param etatcompose_src: L'état source
 * 
 * @return une valeur booléene qui détermine si l'état source existe dans l'ensemble
 * d'états accepteurs de l'automate source ou pas
 **/ 
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

/**
 * Cette fonction permet de vérifier et renvoyer si un état composé passé en paramétre
 * existe dans une liste d'états composés ou pas
 * 
 * @param etat: L'etat composé source
 * @param liste: La liste d'états composés
 * @param taille: La taille de la liste
 * 
 * @return Une valeur booléene permet de déterminer si l'état existe ou pas
 **/
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

/**
 * Cette fonction permet de renvoyer l'état composé équivalente à un état composé 
 * dans une liste d'états.
 * 
 * @param etat: L'état composé référante
 * @param liste: La liste d'états composé
 * @param taille: La taille de la liste
 * 
 * @return Le pointeur vers l'état composé
 **/ 
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

/**
 * Cette fonction permet de renvoyer si un état composé passé en paramétre 
 * existe dans une liste d'états composé ou pas.
 * 
 * @param etat: L'état composé à vérifier
 * @param groupe: La liste d'états composés
 * @param taille: La taille de la liste
 * 
 * @return Une valeur booléene qui permet de déterminer si l'état existe ou pas
 **/ 
bool est_inclut_dans_groupe(EtatCompose etat, EtatCompose* groupe, uint taille) {
    for (size_t i = 0; i < taille; i++) {
        EtatCompose e = groupe[i];
        if (e._index == etat._index)
            return TRUE;
    }

    return FALSE;
}

/**
 * Cette fonction permet de déterminer et renvoyer si un état composé passé
 * en paramétre a un état accepteur d'un AFD passé en paramétre ou pas
 * 
 * @param etat: L'état composé source
 * @param automate_src: L'automate source
 * 
 * @return Une valeur booléene qui permet de déterminer s'il existe un état 
 * accepteur dans l'état composé source ou pas 
 **/ 
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

/**
 * Cette fonction permet de déterminier si deux listes d'états  
 * composés passés en paramétres sont pareils ou pas
 * 
 * @param liste1: Liste d'état composé
 * @param liste2: Liste d'état composé
 * 
 * @return Une valeur booléene qui détermine si ces deux listes sont pareils ou pas
 **/ 
bool is_lec_same(LEtatCompose liste1, LEtatCompose liste2) {
    if (liste1.taille != liste2.taille)
        return FALSE;
    for (size_t i = 0; i < liste1.taille; i++) {
        if (liste1.etats[i]._index != liste2.etats[i]._index)
            return FALSE;
    }
    return TRUE;
}

/**
 * Cette fonction permet de vérifier si une transition AFD existe dans une liste 
 * de transitions passé en paramétre ou pas
 * 
 * @param transition_src: La transition esource
 * @param liste: La liste de transitions
 * @param taille: La taille de la liste
 * 
 * @return Une valeur booléene qui permet de vérifier si la transition existe ou pas
 **/ 
bool does_afdtransition_exist(AFDTransition transition_src, AFDTransition* liste, uint taille) {
    if (taille <= 0 || liste == NULL) return FALSE;
    for (size_t i = 0; i < taille; i++) {
        AFDTransition transition = liste[i];
        if (
            transition_src.e1._index == transition.e1._index &&
            transition_src.e2._index == transition.e2._index &&
            transition_src.alphabet == transition.alphabet
        )
            return TRUE;
    }
    return FALSE;
}

/**
 * Cette fonction permet de construire un état composé 
 * à partir d'un état passé en paramétre
 * 
 * @param state: L'état
 * 
 * @return Un état composé
 **/
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

/**
 * Cette fonction permet de construire un état composé
 * à partir d'une liste d'états composés passé en paramétre
 * 
 * @param liste: La liste d'états composé
 * @param taille: La taille de la liste
 * 
 * @return L'état composé
 **/ 
EtatCompose compose_state_from_group(EtatCompose* liste, uint taille) {
    EtatCompose etat = {._index = rand(), ._size = 0, ._etats = NULL};

    #if DEBUG==1
    printf("\nCOMPOSING a state from group %d [", liste->_index);
    #endif
    for (size_t i = 0; i < taille; i++) {
        EtatCompose ec = liste[i];
        etat._etats = (Etat**) realloc(etat._etats, sizeof(Etat*) * (etat._size + ec._size));
        #if DEBUG==1
        printf("%d {", ec._index);
        #endif
        for (size_t j = 0; j < ec._size; j++) {
            Etat* e = ec._etats[j];
            #if DEBUG==1
            printf("%d, ", e->_index);
            #endif
            etat._etats[etat._size] = e;
            etat._etats[etat._size++]->_index = e->_index;
        }
        #if DEBUG==1
        printf("}, ");
        #endif
    }
    #if DEBUG==1
    printf("]\n");
    #endif
    return etat;
}

/**
 * Cette fonction permet de retirer un état composé d'une liste d'états composés.
 * 
 * @param etat: L'état a retirer
 * @param groupe: Le pointeur vers la liste d'états composés
 * @param taille: Le pointeur vers la taille de la liste
 * 
 * @return La liste 
 **/ 
EtatCompose* retirer_etat_groupe(EtatCompose etat, EtatCompose* groupe, uint* taille) {
    // EtatCompose* altered_group = groupe;
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
            groupe[i] = groupe[i+1];
        }
        *(taille) -= 1;
        // groupe = (EtatCompose*) realloc(groupe, sizeof(EtatCompose) * *(taille));
        #if DEBUG==1
        printf("\tl'état %d a été retirer\n", etat._index);
        #endif
    }

    return groupe;
}
