#ifndef TYPES_H
#define TYPES_H

#define ASCII_LENGTH    128
// #define DEBUG           1

typedef enum _bool bool;

typedef unsigned int uint;

typedef struct _EtatCompose EtatCompose;
typedef struct _Etat Etat;
typedef struct _Transition Transition;
typedef struct _AFDTransition AFDTransition;
typedef struct _AFN AFN;
typedef struct _AFD AFD;

enum _bool {
    FALSE = 0,
    TRUE = 1
};

struct _EtatCompose
{
    Etat** _etats;          // Liste de pointeurs d'états
    uint _size;             // Taille de la liste de pointeurs d'états
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

struct _AFDTransition
{
    EtatCompose e1;        // L'état 1
    EtatCompose e2;        // L'état 2
    char alphabet;          // L'alphabet liant les deux états
};

struct _AFN
{
    Etat** q;               // Ensemble fini de pointeurs d'états
    uint q_size;            // La taille de l'ensemble Q
	Etat* s;                // L'état initial de l'automate
	Etat** f;               // Ensemble de pointeurs d'états accepteurs
	uint f_size;            // La taille de l'ensemble F

	Transition* delta;      // Ensemble de transitions
    uint delta_size;        // La taille de l'ensemble DELTA
};

struct _AFD
{
    EtatCompose* q;         // Ensemble fini de pointeurs d'états composés
    uint q_size;            // La taille de l'ensemble Q
	Etat* s;                // L'état initial de l'automate
	EtatCompose* f;         // Ensemble de pointeurs d'états composés accepteurs
	uint f_size;            // La taille de l'ensemble F

	AFDTransition* sigma;   // Ensemble des transitions
    uint sigma_size;        // La taille de l'ensemble SIGMA
};

#endif
