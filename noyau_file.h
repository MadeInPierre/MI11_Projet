/*----------------------------------------------------------------------------*
 * fichier : noyau_file.h                                                     *
 * gestion de la file d'attente des taches pretes et actives                  *
 * ce fichier declare toutes les primitives de base                           *
 *----------------------------------------------------------------------------*/

#ifndef __NOYAU_FILE_H__
#define __NOYAU_FILE_H__

#include <stdint.h>

/*----------------------------------------------------------------------------*
 * declaration des constantes                                                 *
 *----------------------------------------------------------------------------*/

/*
 * nombre maximum de taches dans le systeme
 */
#define NFILE_MAX_TACHES  8
/*
 * numero de tache impossible, utilise pour savoir si la file est initialisee
 * ou non
 */
#define F_VIDE      NFILE_MAX_TACHES


/*
 * Structure de déclaration d'une file. Plusieurs files peuvent être créées.
 */
typedef struct {
	uint16_t _queue;
    uint16_t _file[NFILE_MAX_TACHES];
} FIFO_NOYAU;



/*----------------------------------------------------------------------------*
 * prototypes des fonctions de gestion de la file                             *
 * voir le fichier noyau_file.c pour avoir le comportement des fonctions      *
 *----------------------------------------------------------------------------*/

void file_init(FIFO_NOYAU *f);
void file_ajoute(FIFO_NOYAU *f, uint16_t n);
void file_retire(FIFO_NOYAU *f, uint16_t t);
int file_vide(FIFO_NOYAU *f);
uint16_t file_suivant(FIFO_NOYAU *f);
void file_affiche_queue(FIFO_NOYAU *f);
void file_affiche(FIFO_NOYAU *f);


#endif //__NOYAU_FILE_H__
