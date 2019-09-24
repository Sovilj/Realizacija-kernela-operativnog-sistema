/*
 * semlist.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: OS1
 */


#include "semlist.h"

SemList::SemList(){
	lock;
	head = tail = 0;
	cnt = 0;
	unlock;
}
SemList::~SemList(){
	lock;
	Elem* temp = head;
	while(temp){
		head = head->next;
		delete temp->sem;
		delete temp;
		temp = head;
	}
	head = tail = 0;
	cnt = 0;
	unlock;
}
void SemList::put(KernelSem* s){
	lock;
	Elem * tek = head;
	while( tek && tek->sem != s ){
		tek = tek->next;
	}
	if (tek!=0){
		unlock;
		return;
	}
	if (!head){
			head = new Elem(s);
			tail = head;
	}else{
			tail = tail->next = new Elem(s);
	}
	cnt++;
	unlock;
}
void SemList::remove(KernelSem* s){
	lock;
	Elem *tek = head;
	Elem *pre = 0;
	while( tek && tek->sem != s){		// trazimo s
		pre = tek;
		tek = tek->next;
	}
	if ( tek ){							// ako nisam nasao->kraj
		if ( pre ){						// ako postoji prev znaci da
			pre->next = tek ->next;		// se ne radi o prvom elem
			if ( pre->next == 0)		// prespajam i proverama da li
				tail = pre;				// je to kraj liste pa updateujem
										// tail
		}else
			head = head->next;
		cnt--;
		delete tek;
	}
	unlock;
}
KernelSem* SemList::get_by_id(int i){
	lock;
	Elem *tek = head;
	while( tek && tek->sem->get_id() != i){
		tek = tek->next;
	}
	if ( tek != 0){
		unlock;
		return tek->sem;
	}else
	{
		unlock;
		return 0;
	}
}
KernelSem* SemList::get_by_place(int i){
	lock;
	Elem* tek = head;
	if ( tek == 0){ unlock; return 0;}
	for ( int j = 0 ; j< i; j++)
		tek = tek->next;
	if (tek !=0){
		unlock;
		return tek->sem;
	}else{
		unlock;
		return 0;
	}
}
int SemList::is_empty(){
	lock;
	if ( head == 0 && cnt == 0){
		unlock; return 1;}
	else{
		unlock; return 0;}
}
int SemList::size(){
	lock;
	int s = cnt;
	unlock;
	return s;
}

void SemList::update_all(){

	if ( is_empty() == 1){ return; } 	// nema semafora
	lock;
	Elem * temp = head;
	while ( temp!=0 ){     			 // prolazimo kroz listu semafora i svaki refreshujemo
		temp->sem->refresh(); 	   	 // ako nema blokiranih na semaforu refresh
		temp = temp->next;			 // samo izadje a ako ima smanji vreme svim nitima za 1 izbaci niti koje su na time= 0
	}
	unlock;
}


