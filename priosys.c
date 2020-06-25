/*
 * priosys.c
 *
 *  Created on: Jun 25, 2020
 *      Author: jack
 */

#include "priosys.h"
#include "noyau_file.h"

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

void _decompose_id(uint16_t id, uint16_t *prio, uint16_t *local_id) {
	*prio = id / NB_PRIOS;
	*local_id = id % NB_PRIOS;
//	printf("Decomp id=%d p=%d i=%d\n", id, *prio, *local_id);
}


void priosys_init() {
	for(int i = 0; i < NB_PRIOS; i++) {
		file_init(&tourniquets[i]);
	}
}

uint16_t priosys_generate_next_identity(uint16_t prio) {
	for(int i = 0; i < NFILE_MAX_TACHES; i++) {
		if((identities[prio] & (1 << i)) == 0) {
			identities[prio] |= 1 << i; // Mark this identity as created
			occupations      |= 1 << prio; // Mark this priority as not empty
			printf("New p=%d id=%d occ=%d\n", prio, _generate_id(prio, i), occupations);
			return _generate_id(prio, i);
		}
	}
	return NB_TACHES;
}

void priosys_add_task(uint16_t id) {
	uint16_t prio, local_id;
	_decompose_id(id, &prio, &local_id);
	file_ajoute(&tourniquets[prio], local_id);

	// Mark this priority as not empty
	occupations |= 1 << prio;
}

void priosys_remove_task(uint16_t id) {
	uint16_t prio, local_id;
	_decompose_id(id, &prio, &local_id);
	file_retire(&tourniquets[prio], local_id);

	// Mark this priority as empty if we removed the last id
	if(file_vide(&tourniquets[prio]))
		occupations &= ~(1 << prio);
}

uint16_t priosys_next() {
	/* pour chaque bit de occupation
	 * si il est à 1
	 * 		alors la file de priorité i n'est pas vide
	 * 		donc il y a une tâche active de cette priorité
	 * on prend la prochaîne valeur dans la file i
	 * c'est l'id de la tâche à exécuter, on le retourne
	 */

	for(int prio = 0; prio < NB_PRIOS; prio++)
		if((occupations & (1 << prio)) != 0)
			return _generate_id(prio, file_suivant(&tourniquets[prio]));

	return NB_TACHES;
}







