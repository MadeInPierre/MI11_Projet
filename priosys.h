/*
 * priosys.h
 *
 *  Created on: Jun 25, 2020
 *      Author: jack
 */

#ifndef PRIOSYS_H_
#define PRIOSYS_H_

#include "noyau_file.h"

#define NB_PRIOS 8
#define NB_TACHES (NB_PRIOS*NFILE_MAX_TACHES)

// Initialisation des files de priorités
void     priosys_init();

// Génération d'une nouvelle identité en fonction
// Des tâches déjà créées.
uint16_t priosys_generate_next_identity(uint16_t prio);

// Ajoute une tâche (la priorité est déduite de l'identité)
// à la file de priorité appropriée.
void     priosys_add_task(uint16_t id);

// Retire une tâche (la priorité est déduite de l'identité)
// de la file de priorité appropriée.
void     priosys_remove_task(uint16_t id);

// Sélectionne la tâche active de priorité la plus forte, en
// Faisant tourner la file en cas d'égalités.
uint16_t priosys_next();

#endif /* PRIOSYS_H_ */
