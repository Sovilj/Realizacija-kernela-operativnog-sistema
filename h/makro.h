/*
 * makro.h
 *
 *  Created on: Aug 2, 2017
 *      Author: OS1
 */

#ifndef MAKRO_H_
#define MAKRO_H_

#define lock { asm pushf ; asm cli;}
#define unlock {asm popf;}

#define int_timer 0x08				// prekid od tajmera
#define int_mask 0x0200				// maska za PSW I=1

#define NEW 1						// stanja kroz koje prolazi nit
#define READY 2
#define ACTIVE 3
#define BLOCKED 4
#define FINISHED 5

#define min_time_slice 55
#define max_stack_size 65536
#define max_ivt_entries 256

#endif /* MAKRO_H_ */
