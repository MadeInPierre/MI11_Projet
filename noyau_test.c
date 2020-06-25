/*----------------------------------------------------------------------------*
 * fichier : noyau_test.c                                                     *
 * programme de test du noyaut                                                *
 *----------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdlib.h>

#include "serialio.h"
#include "noyau.h"

#include "TERMINAL.h"
#include "utils.h"
#include "FIFO_GEN.h"

// TACHE A - PRIO = 1, SLEEP = 30, WORK = 10
// TACHE B - PRIO = 2, SLEEP =  5, WORK =  2
// TACHE C - PRIO = 2, SLEEP = 10, WORK =  5
// TACHE D - PRIO = 2, SLEEP = 15, WORK =  3
// TACHE E - PRIO = 5, SLEEP =  5, WORK = 20
#define TACHE_A_SLEEP 30
#define TACHE_A_WORK  10
#define TACHE_B_SLEEP 5
#define TACHE_B_WORK  2
#define TACHE_C_SLEEP 10
#define TACHE_C_WORK  5
#define TACHE_D_SLEEP 15
#define TACHE_D_WORK  3
#define TACHE_E_SLEEP 5
#define TACHE_E_WORK  20

TACHE	tacheMAIN(void);
TACHE	tacheA(void);
TACHE	tacheB(void);
TACHE	tacheC(void);
TACHE	tacheD(void);
TACHE	tacheE(void);

#define NB_MESSAGES 1024
#define TAILLE_MESSAGE 128

FIFO_GEN fifo_messages;

char messages[NB_MESSAGES*TAILLE_MESSAGE];

// Fonction d'aide permettant d'ajouter un message à
// imprimer par la tâche de fond. Imprime l'id de la
// tâche et une couleur propre.
void tache_msg(uint16_t *idx, uint16_t color, char* name, int i) {
	static char temp_string[100];
	uint16_t b_idx = *idx;

	_lock_();
	if(fifogen_get(&fifo_messages, &b_idx) != 0){
		messages[b_idx*TAILLE_MESSAGE] = '\0';
		str_cat(&messages[b_idx*TAILLE_MESSAGE], CODE_BACKGROUND_COLOR);
		i_to_a10 (color,temp_string);
		str_cat(&messages[b_idx*TAILLE_MESSAGE], temp_string);
		str_cat(&messages[b_idx*TAILLE_MESSAGE], "m ");
//		str_cat(&messages[b_idx*TAILLE_MESSAGE], name);
		i_to_a10(noyau_get_tc(),temp_string);
		str_cat(&messages[b_idx*TAILLE_MESSAGE], temp_string);
		str_cat(&messages[b_idx*TAILLE_MESSAGE], CODE_RESET_COLOR);
	}
	*idx = b_idx;
	_unlock_();
}


TACHE tacheMAIN(void) {
	uint16_t idx;

	puts("------> EXEC tache M");
	puts("\n\n*******************************");
	puts("*     Pierre LACLAU, GI04     *");
	puts("*******************************\n\n");

	active(cree(tacheA, 1));
	active(cree(tacheB, 2));
	active(cree(tacheC, 2));
	active(cree(tacheD, 2));
	active(cree(tacheE, 5));

	while(1){
		_lock_();
		if(fifogen_taille_get(&fifo_messages) != 0){
			fifogen_put(&fifo_messages, &idx);
			printf(&messages[idx*TAILLE_MESSAGE]);
		}
		_unlock_();

		while(flag_tache_vide != 0){
			tache_msg(&idx, 16, "M", idx);
			flag_tache_vide = 0;
		}
	}
}

TACHE tacheA(void) {
	int i=0;
	uint16_t idx;

	puts("------> DEBUT tache A");
	while (1) {
		if(flag_tache_vide != 0) {
			tache_msg(&idx, 39, "A", i++);
			flag_tache_vide = 0;
		}

		if(i % TACHE_A_WORK == 0)
			waitfornticks(TACHE_A_SLEEP);
	}
}

TACHE tacheB(void) {
	int i=0;
	uint16_t idx;

	puts("------> DEBUT tache B");
	while (1) {
		if(flag_tache_vide != 0) {
			tache_msg(&idx, 88, "B", i++);
			flag_tache_vide = 0;
		}

		if(i % TACHE_B_WORK == 0)
			waitfornticks(TACHE_B_SLEEP);
	}
}

TACHE tacheC(void) {
	int i=0;
	uint16_t idx;

	puts("------> DEBUT tache C");
	while (1) {
		if(flag_tache_vide != 0) {
			tache_msg(&idx, 53, "C", i++);
			flag_tache_vide = 0;
		}

		if(i % TACHE_C_WORK == 0)
			waitfornticks(TACHE_C_SLEEP);
	}
}

TACHE tacheD(void) {
	int i = 0;
	uint16_t idx;

	puts("------> DEBUT tache D");
	while (1) {
		if(flag_tache_vide != 0) {
			tache_msg(&idx, 21, "D", i++);
			flag_tache_vide = 0;
		}

		if(i % TACHE_D_WORK == 0)
			waitfornticks(TACHE_D_SLEEP);

		if (i==300) noyau_exit();
	}
}

TACHE tacheE(void) {
	int i = 0;
	uint16_t idx;

	puts("------> DEBUT tache E");
	while (1) {
		if(flag_tache_vide != 0) {
			tache_msg(&idx, 142, "E", i++);
			flag_tache_vide = 0;
		}

		if(i % TACHE_E_WORK == 0)
			waitfornticks(TACHE_E_SLEEP);
	}
}

int main() {
	serial_init(0, 115200);

	test_colors();

	puts("Test noyau");
	puts("Noyau preemptif");

	fifogen_init(&fifo_messages, NB_MESSAGES);

	start(tacheMAIN, 7);
	return(0);
}
