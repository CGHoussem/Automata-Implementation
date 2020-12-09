#ifndef TYPES_H
#define TYPE_H

typedef unsigned int uint;

// typedef struct _Etat Etat;
typedef struct _Transition Transition;
typedef struct _AFN AFN;

// struct _Etat
// {
//     uint _index;            // Identifiant de l'état
// };

struct _Transition 
{
    uint e1;                // Identifiant de l'état 1
    uint e2;                // Identifiant de l'état 2
    char alphabet;          // L'alphabet liant
};

struct _AFN
{
    uint* q;                // Ensemble fini des identifiants des états
    uint q_size;            // La taille des états
    char alphabet;          // L'alphabet de l'automate
	uint s;                 // L'identifiant de l'état initial de l'automate
	uint* f;                // Ensemble des identifiants des états accepteurs de l'automate
	uint f_size;            // La taille des états finaux

	Transition* delta;      // Les transitions
    uint delta_size;        // La taille de la liste des transitions
};

#endif
