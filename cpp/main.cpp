/*
 * main.cpp
 *
 * Created on: Aug 3, 2017
 *      Author: OS1
 */

#include <iostream.h>
#include "makro.h"
#include "system.h"

int userMain(int argc, char* argv[] );

int main(int argc , char* argv[]){
	lock;
	cout<< "Main start"<<endl;
	unlock;

	System::start();
	int ret_val = userMain(argc,argv);
	dispatch();
    System::stop();

    lock;
	cout<< "Main stop"<<endl;
	unlock;

	return ret_val;
}
