/*
 * system.cpp
 *
 *  Created on: Aug 5, 2017
 *      Author: OS1
 */

#include "system.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "makro.h"
#include "thread.h"
#include "idlethr.h"
#include "kernsem.h"

unsigned System::expl_flag = 0;
unsigned System::lock_flag = 0;
Time System::cur_time = 0;

PCB* System::main_pcb = 0;
Thread* System::idle_thread = 0;
PCB* System::idle_pcb = 0;

unsigned int oldTimerOFF, oldTimerSEG;
unsigned int newTimerOFF, newTimerSEG;
unsigned int tss,tsp,tbp;

void System::start(){

	System::main_pcb = new PCB(0,defaultStackSize,defaultTimeSlice);	// (pravimo main thread od default parametara)pravimo samo main_pcb bez main niti
	System::main_pcb->set_status(READY)	;
	PCB::running = System::main_pcb;				// running je = main kao jedina nit koja trenutno postoji

	System::idle_thread = new IdleThread();
	System::idle_pcb = PCB::all_pcb->get_by_id(System::idle_thread->getId());

	System::idle_thread->start();

	System::init();
	lock;
	cout<< "sistem start"<<endl;
	unlock;
}
void System::stop(){

	System::rest();
	delete main_pcb;
	delete idle_thread;
	while ( !PCB::all_pcb->is_empty()){
		PCB* temp = PCB::all_pcb->get();
		delete temp;
	}
	lock;
	cout<< "sistem stop"<<endl;
	unlock;
}

void System::init(){
#ifndef BCC_BLOCK_IGNORE
	newTimerSEG = FP_SEG(System::timer);		// uzimam seg i off od timer metode
	newTimerOFF = FP_OFF(System::timer);
#endif
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h				// pamtimo staru rutinu
		mov word ptr oldTimerSEG, ax			// jer namecemo nasu na njeno mesto
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		//mov word ptr es:0022h, seg timer		// ne prepoznaje seg i offset f-je
		//mov word ptr es:0020h, offset timer	//

		mov ax, newTimerSEG						// ovo umesto 2 reda iznad
		mov word ptr es:0022h, ax				// postavlja novu rutinu na mesto stare
		mov ax, newTimerOFF
		mov word ptr es:0020h, ax

		mov ax, oldTimerSEG						// staru rutinu pamtimo na int 60h
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h,ax

		pop ax
		pop es
		sti
	};

}
void System::rest(){

	asm {						// kada gasimo sistem
		cli						// zelimo da vratimo kako je bilo
		push es					// pa vracamo staru prekidnu rutinu
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	};
}
void interrupt System::timer(){

	if ( System::expl_flag == 0){
		asm int 60h;									// ako nije explicitno trazena promena
		if ( KernelSem::all_sem->is_empty() != 1)		// zovemo timer koji je sada na int 60
			KernelSem::all_sem->update_all();			// i sve semafore update-ujemo
		tick();
	}
	// ako je eksplicitno trazena promena ili ako je isteklo vreme niti i lockflag je na 0
	// radimo promenu konteksta
	if (System::expl_flag ==1 ||  ((System::cur_time+1)== PCB::running->get_time_slice()) ) {

		if( System::lock_flag == 0){

			System::cur_time = 0;
			System::expl_flag = 0;

			#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tbp, bp
				mov tss, ss
				mov tsp, sp
			};
			#endif
			PCB::running->sp=tsp;				// pamtimo trenutne ss,sp,bp (kontekst procesora)
			PCB::running->ss=tss;				// da bi posle mogli da nastavimo odatle
			PCB::running->bp=tbp;

			if( PCB::running->get_status()==ACTIVE && PCB::running!=System::idle_pcb){
				PCB::running->set_status(READY);
				Scheduler::put(PCB::running);
			}
			PCB::running = Scheduler::get();

			if(PCB::running == 0 ){
				PCB::running = System::idle_pcb;
			}
			PCB::running->set_status(ACTIVE);

			tsp = PCB::running->sp;
			tbp = PCB::running->bp;
			tss = PCB::running->ss;

			#ifndef BCC_BLOCK_IGNORE
			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			};
			#endif

		}
	}else
	{ 		// ako dispatch nije pozvana expl. i ako timeslice nije beskonacan tj jednak 0
			// cur time ostaje na 0 i nece se raditi promena konteksta sve dok se ne odradi explicitno ili prekidom

		if( System::lock_flag==0 && PCB::running->get_time_slice()!=0){
			System::cur_time++;
		}
	}

}
// za potrebe testiranja
//void tick(){}

void lockFlag(){
	System::lock_flag = 1;
}
void unlockFlag(){
	System::lock_flag = 0;
}


