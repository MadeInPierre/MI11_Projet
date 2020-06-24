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
#define TACHE_B_WORK  100
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


// Helper function used to append a message log when
// the caller task is executed.
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
		str_cat(&messages[b_idx*TAILLE_MESSAGE], name);
//		i_to_a10 (i,temp_string);
//		str_cat(&messages[b_idx*TAILLE_MESSAGE], temp_string);
		str_cat(&messages[b_idx*TAILLE_MESSAGE], CODE_RESET_COLOR);
	}
	*idx = b_idx;
	_unlock_();
}


TACHE tacheMAIN(void) {
	uint16_t idx;
	//char temp_string[100];

	puts("------> EXEC tache M");
	active(cree(tacheA));
	active(cree(tacheB));
	active(cree(tacheC));
	active(cree(tacheD));
	active(cree(tacheE));

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
//			_lock_();
//			if(fifogen_get(&fifo_messages, &idx) != 0){
//				messages[idx*TAILLE_MESSAGE] = '\0';
//				str_cat(&messages[idx*TAILLE_MESSAGE], CODE_BACKGROUND_COLOR);
//				i_to_a10 (16,temp_string);
//				str_cat(&messages[idx*TAILLE_MESSAGE], temp_string);
//				str_cat(&messages[idx*TAILLE_MESSAGE], "m A");
//				i_to_a10 (idx,temp_string);
//				str_cat(&messages[idx*TAILLE_MESSAGE], temp_string);
//				str_cat(&messages[idx*TAILLE_MESSAGE], CODE_RESET_COLOR);
//			}
//			flag_tache_vide = 0;
//			_unlock_();
		}
	}
}

TACHE tacheA(void) {
	int i=0;
	uint16_t idx;

	puts("------> DEBUT tache B");
	while (1) {
		tache_msg(&idx, 39, "A", i++);
		waitfornticks((i % TACHE_A_WORK == 0) ? TACHE_A_SLEEP : 1);
	}
}

TACHE tacheB(void) {
	int i=0;
	uint16_t idx;

	puts("------> DEBUT tache B");
	while (1) {
		tache_msg(&idx, 88, "B", i++);
		waitfornticks((i % TACHE_B_WORK == 0) ? TACHE_B_SLEEP : 1);
	}
}

TACHE tacheC(void) {
	int i=0;
	uint16_t idx;

	puts("------> DEBUT tache C");
	while (1) {
		tache_msg(&idx, 53, "C", i++);
		waitfornticks((i % TACHE_C_WORK == 0) ? TACHE_C_SLEEP : 1);
	}
}

TACHE tacheD(void) {
	int i = 0;
	uint16_t idx;

	puts("------> DEBUT tache D");
	while (1) {
		tache_msg(&idx, 21, "D", i++);
		waitfornticks((i % TACHE_D_WORK == 0) ? TACHE_D_SLEEP : 1);

		if (i==50) noyau_exit();
	}
}

TACHE tacheE(void) {
	int i = 0;
	uint16_t idx;

	puts("------> DEBUT tache E");
	while (1) {
		tache_msg(&idx, 142, "E", i++);
		waitfornticks((i % TACHE_E_WORK == 0) ? TACHE_E_SLEEP : 1);
	}
}

int main() {
	serial_init(0, 115200);

	test_colors();

	puts("Test noyau");
	puts("Noyau preemptif");

	fifogen_init(&fifo_messages, NB_MESSAGES);

	start(tacheMAIN);
	return(0);
}
