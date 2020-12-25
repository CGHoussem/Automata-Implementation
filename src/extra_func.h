#ifndef EXTRA_FUNC_H
#define EXTRA_FUNC_H

#include "types.h"

//
void append_to_transitions(Transition*, Transition);
void append_Q_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state);
void append_F_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state);
void append_DELTA_to_AFN(AFN* automate_dest, AFN automate_src);

//
Transition** get_transitions_from_AFN(AFN automate_src, Etat* state_src);

//
bool is_state_final(AFN automate_src, Etat* etat);
bool existe_etatcompose_AFD_Q(AFD automate_src, EtatCompose etats_src);
void add_to_EtatCompose(EtatCompose* composed_state, Etat* state);

//
Etat* create_state();
EtatCompose compose_state(Etat* state);


#endif