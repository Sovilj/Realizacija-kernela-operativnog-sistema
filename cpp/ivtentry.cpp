/*
 * ivtentry.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: OS1
 */


#include "ivtentry.h"
#include "makro.h"
#include <dos.h>
#include <iostream.h>
#include <stdlib.h>

IVTEntry* IVTEntry::all_ivt[max_ivt_entries] = {0};

IVTEntry::IVTEntry(IVTNo ivtno, InterruptRoutine routine){
	lock;
	ivt_num = ivtno;
	if ( all_ivt[ivtno]== 0){					// dodajem entry na listu svih
		all_ivt[ivtno] = this;
	}
	old_routine = 0;    						// samo zbog warninga...BCC_BLOCK_IGNORE
#ifndef BCC_BLOCK_IGNORE
	old_routine = getvect(ivtno); 				// vraca adresu inter. f-je na broju ivtno
	setvect(ivtno,routine);  					// postavlja novu inter. rutinu na mesto broja ivtno
#endif
	unlock;
}
IVTEntry::~IVTEntry(){
	lock;
	if ( IVTEntry::all_ivt[ivt_num] != 0){
		IVTEntry::all_ivt[ivt_num]=0;
	}
#ifndef BCC_BLOCK_IGNORE
	setvect(ivt_num , old_routine);  			// vracamo staru rutinu koja je bila na ovom broju
#endif
	unlock;
}
