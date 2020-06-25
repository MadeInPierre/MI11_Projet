/*
 * priosys.c
 *
 *  Created on: Jun 24, 2020
 *      Author: jack
 */

#include "noyau_file.h"
#include "priosys.h"
#include "noyau.h"
#include "serialio.h"
#include <stdint.h>

// FIFOs pour chacune des priorites. Chaque FIFO
// stocke l'ordre d'exécution des tâches de meme priorite,
// en stockant l'identité des tâches.
FIFO_NOYAU tourniquets[NB_PRIOS];

// Pour chaque tourniquet, un octet représente le masque des
// identités déjà attribuées.
uint8_t identities[NB_PRIOS];

// Représente un masque des FIFOs non vides.
uint8_t occupations;

uint16_t _generate_id(uint16_t prio, uint16_t fifo_n) {
	return NB_PRIOS * prio + fifo_n;
}

void _decompose_id(uint16_t id, uint16_t *prio, uint16_t *fifo_n) {
	*prio = id / NB_PRIOS;
	*fifo_n = id % NB_PRIOS;
}

uint16_t priosys_next() {
	/* pour chaque bit de occupation
	 * si il est à 1
	 * 		alors la file de priorité i n'est pas vide
	 * 		donc il y a une tâche active de cette priorité
	 * on prend la prochaîne valeur dans la file i
	 * c'est l'id de la tâche à exécuter, on le retourne
	 */

	for(int i = 0; i < NB_PRIOS; i++) {
		if(((occupations >> i) & 0x01) == 1)
			return file_suivant(&tourniquets[i]);
	}

	return NB_TACHES;
}

void priosys_remove_task(uint16_t id) {
	uint16_t prio, i_fifo;
	_decompose_id(id, &prio, &i_fifo);
	file_retire(&tourniquets[prio], id);

	// Si la file est maintenant vide, on met le bit d'occupation à 0
	if(file_vide(&tourniquets[prio]))
		occupations &= ~(1 << prio);
}

void priosys_add_task(uint16_t id) {
	uint16_t prio, i_fifo;
	_decompose_id(id, &prio, &i_fifo);
	file_ajoute(&tourniquets[prio], id);
}

uint16_t priosys_generate_next_identity(uint16_t prio) {
	for(int i = 0; i < MAX_TACHES; i++) {
		if((identities[prio] & (1 << i)) == 0) {
			// Mark this identity as created
			identities[prio] |= 1 << i;

			// Mark this priority as not empty
			occupations |= 1 << prio;

			printf("Nouveau p=%d id=%d occ=%d\n", prio, _generate_id(prio, i), occupations);
			return _generate_id(prio, i);
		}
	}
	return NB_TACHES;
}

void priosys_init() {
	for(int i = 0; i < NB_PRIOS; i++) {
		file_init(&tourniquets[i]);
	}
}
