/*
 * priosys.h
 *
 *  Created on: Jun 24, 2020
 *      Author: jack
 */

#ifndef PRIOSYS_H_
#define PRIOSYS_H_

#include "noyau_file.h"

#define NB_PRIOS 8

#define NB_TACHES (NB_PRIOS*MAX_TACHES)

void     priosys_init();
uint16_t priosys_generate_next_identity(uint16_t prio);
void  priosys_add_task(uint16_t id);
void  priosys_remove_task(uint16_t id);
uint16_t priosys_next();

#endif /* PRIOSYS_H_ */
