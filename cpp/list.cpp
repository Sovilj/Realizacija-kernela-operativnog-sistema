/*
 * list.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: OS1
 */
#include <stdlib.h>
#include "list.h"
#include "makro.h"

List::List(){
	lock;
	head = 0;
	tail = 0;
	cnt = 0;
	unlock;
}

List::~List(){
	lock;
	Elem *temp = head;
	while(temp != 0){					// iteriram dok postoje elementi
		head = head->next;				// i brisem jedan po jedan
		delete temp;
		temp = head;
	}
	head = tail = 0;
	cnt = 0;
	unlock;

}
void List::put(PCB*p){
	lock;
	Elem *tek = head;
	while(tek && tek->data!=p){			// trazim da li vec imam u listi PCB p
		tek = tek->next;
	}
	if (tek != 0){					    // ako ima isti samo izadjemo jer
		unlock;							// vec postoji u listi
		return;
	}
	if (!head){							//dodajemo na kraj
		head = new Elem(p);
		tail = head;
	}else{
		tail = tail->next = new Elem(p);
	}
	cnt++;
	unlock;
}
PCB* List::get_by_id(int i){
	lock;
	Elem* temp = head;
	while (temp->data->get_id() != i)
		temp = temp->next;
	if ( temp != 0){
		unlock;
		return temp->data;
	}else{
		unlock;
		return 0;
	}
}
void List::remove(PCB* p){
	lock;
	Elem * temp = head;
	Elem * prev = 0;
	while( temp != 0 && temp->data != p){			// trazim PCB p ali
		prev = temp;								// pamtim i prethodnika
		temp = temp->next;
	}
	if ( temp!=0 ){									// ako nisam nasao->kraj
		if ( prev ){								// ako postoji prev znaci da
			prev->next = temp ->next;				// se ne radi o prvom elem
			if ( prev->next == 0)					// prespajam i proverama da li
				tail = prev;						// je to kraj liste pa updateujem
													// tail
		}else
			head = head->next;

		cnt--;
		delete temp;
	}
	unlock;
}
PCB* List::get(){
	if ( head == 0) return 0;
	lock;
	Elem* temp = head;
	PCB* p = temp->data;
	head = head->next;
	if(!head)
		tail = 0;
	delete temp;
	cnt--;
	unlock;
	return p;
}
PCB* List::get_by_place(int i){
	if ( head == 0) { return 0;}
	lock;
	Elem * tek = head;
	for ( int j = 0 ; j < i; j++){
		tek = tek->next;
	}
	if( tek!=0 ){
		unlock;
		return tek->data;
	}else
	{
		unlock;
		return 0;
	}
}

int List::is_empty(){
	lock;
	if (head == 0 && cnt == 0) {
		unlock; return 1;}
	else {
		unlock; return 0; }
}
int List::size(){
	lock;
	int i = cnt;
	unlock;
	return i;
}
