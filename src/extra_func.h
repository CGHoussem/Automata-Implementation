#ifndef EXTRA_FUNC_H
#define EXTRA_FUNC_H

#include "types.h"

//
void append_to_transitions(AFNTransition*, AFNTransition);
void append_Q_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state);
void append_F_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state);
void append_DELTA_to_AFN(AFN* automate_dest, AFN automate_src);

//
AFNTransition** get_transitions_from_AFN(AFN automate_src, Etat* state_src);
AFDTransition** get_transitions_from_AFD_EtatComp(AFD automate_src, EtatCompose state_src);
AFDTransition** get_transitions_from_AFD(AFD automate_src, Etat* state_src);
LEtatCompose get_destinations_from_EtatCompose_AFD(AFD automate_src, EtatCompose etatcompose_src);
EtatCompose* get_group(EtatCompose etat, EtatCompose** groupes, uint taille_groupes, uint* taille_listes);

//
bool is_state_final(AFN automate_src, Etat* etat);
bool existe_etatcompose_AFD_Q(AFD automate_src, EtatCompose etats_src);
bool existe_etatcompose_AFD_F(AFD automate_src, EtatCompose etatcompose_src);
bool est_inclut_dans_groupe(EtatCompose etat, EtatCompose* groupe, uint taille);
bool has_final_state(EtatCompose etat, AFD automate_src);

//
Etat* create_state();
EtatCompose* compose_state(Etat* state);
EtatCompose compose_state_from_group(EtatCompose* liste, uint taille);
EtatCompose* retirer_etat_groupe(EtatCompose etat, EtatCompose* groupe, uint* taille);


#endif