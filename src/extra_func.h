#ifndef EXTRA_FUNC_H
#define EXTRA_FUNC_H

#include "types.h"

void append_to_transitions(Transition*, Transition);
void append_Q_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state);
void append_F_to_AFN(AFN* automate_dest, AFN automate_src, bool include_initial_state);
void append_DELTA_to_AFN(AFN* automate_dest, AFN automate_src);

bool is_state_final(AFN automate_src, Etat* etat);

Etat* create_state();

#endif