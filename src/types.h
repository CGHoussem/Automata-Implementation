#ifndef TYPES_H
#define TYPES_H

#define ASCII_LENGTH    128

typedef enum _bool bool;

typedef unsigned int uint;

typedef struct _Etat Etat;
typedef struct _Transition Transition;
typedef struct _AFN AFN;

enum _bool {
    FALSE = 0,
    TRUE = 1
};

struct _Etat
{
    uint _index;            // Identifiant de l'état
};

struct _Transition 
{
    Etat* e1;               // L'état 1
    Etat* e2;               // L'état 2
    char alphabet;          // L'alphabet liant les deux états
};

struct _AFN
{
    Etat** q;               // Ensemble fini des états
    uint q_size;            // La taille des états
	Etat* s;                // L'état initial de l'automate
	Etat** f;               // Ensemble des états accepteurs de l'automate
	uint f_size;            // La taille des états finaux

	Transition* delta;      // Les transitions
    uint delta_size;        // La taille de la liste des transitions
};

#endif
