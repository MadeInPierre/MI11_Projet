/*----------------------------------------------------------------------------*
 * fichier : noyau_file.c                                                     *
 * gestion de la file d'attente des taches pretes et actives                  *
 * la file est rangee dans un tableau. ce fichier definit toutes              *
 * les primitives de base                                                     *
 *----------------------------------------------------------------------------*/

#include "noyau_file.h"
#include <stdint.h>
// recuperation du bon fichier selon l'architecture pour la fonction printf
#ifdef __arm__
#include "serialio.h"
#endif
#ifdef __x86_64__
#include <stdio.h>
#endif

/*----------------------------------------------------------------------------*
 * variables communes a toutes les procedures                                 *
 *----------------------------------------------------------------------------*/

/*
 * tableau qui stocke les taches
 * indice = numero de tache
 * valeur = tache suivante
 */
//static uint16_t _file[MAX_TACHES];

/*
 * index de queue
 * valeur de l'index de la tache en cours d'execution
 * pointe sur la prochaine tache a activer
 */
//static uint16_t _queue;

/*----------------------------------------------------------------------------*
 * fonctions de gestion de la file                                            *
 *----------------------------------------------------------------------------*/

/*
 * initialise la file
 * entre  : sans
 * sortie : sans
 * description : la queue est initialisee à une valeur de tache impossible
 */
void file_init(FIFO_NOYAU *f) {
    f->_queue = F_VIDE;
}

/*
 * ajoute une tache dans la file
 * entre  : n numero de la tache a ajouter
 * sortie : sans
 * description : ajoute la tache n en fin de file
 */
void file_ajoute(FIFO_NOYAU *f, uint16_t n) {
    if (f->_queue == F_VIDE) {
    	f->_file[n] = n;
    } else {
    	f->_file[n] = f->_file[f->_queue];
    	f->_file[f->_queue] = n;
    }

    f->_queue = n;
}

/*
 * retire une tache de la file
 * entre  : t numero de la tache a retirer
 * sortie : sans
 * description : retire la tache t de la file. L'ordre de la file n'est pas
                 modifie
 */
void file_retire(FIFO_NOYAU *f, uint16_t t) {
    if (f->_queue == f->_file[f->_queue]) {
    	f->_queue = F_VIDE;
    } else {
        if (t == f->_queue) {
        	f->_queue = f->_file[f->_queue];
            while (f->_file[f->_queue] != t) {
            	f->_queue = f->_file[f->_queue];
            }
            f->_file[f->_queue] = f->_file[t];
        } else {
            while (f->_file[f->_queue] != t) {
            	f->_queue = f->_file[f->_queue];
            }
            f->_file[f->_queue] = f->_file[f->_file[f->_queue]];
        }
    }
}

/*
 * donne l'etat de la file, vide ou non
 * entre  : sans
 * sortie : 1 si la tâche est vide, 0 sinon
 * description : test file vide
 */
int file_vide(FIFO_NOYAU *f) {
	return (f->_queue == F_VIDE) ? 1 : 0;
}

/*
 * recherche la tache suivante a executer
 * entre  : sans
 * sortie : numero de la tache a activer
 * description : queue pointe sur la tache suivante
 */
uint16_t file_suivant(FIFO_NOYAU *f) {
    if (f->_queue != F_VIDE) {
    	f->_queue = f->_file[f->_queue];
    }
    return (f->_queue);
}

/*
 * affiche la queue, donc la derniere tache
 * entre  : sans
 * sortie : sans
 * description : affiche la valeur de queue
 */
void file_affiche_queue(FIFO_NOYAU *f) {
    printf("_queue = %d\n", f->_queue);
}

/*
 * affiche la file
 * entre  : sans
 * sortie : sans
 * description : affiche les valeurs de la file
 */
void file_affiche(FIFO_NOYAU *f) {
    int i;

    printf("Tache   | ");
    for (i = 0; i < NFILE_MAX_TACHES; i++) {
        printf("%03d | ", i);
    }

    printf("\nSuivant | ");
    for (i = 0; i < NFILE_MAX_TACHES; i++) {
        printf("%03d | ", f->_file[i]);
    }
    printf("\n");
}
