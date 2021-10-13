#include "./../TestInterface.h"

#ifdef TestPthread

#include "pthread.h"

#include <iostream>
using namespace std;

void* Function_t(void* Param)  
{  
	pthread_t myid = pthread_self();  
	cout << "线程ID:" << myid.x << endl;  
	return NULL;
} 

int test_interface(int argc, char ** argv)
{
	pthread_t pid;  
	pthread_attr_t attr;  
	pthread_attr_init(&attr);  
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);  
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 

	cout << "创建线程" << endl;
	pthread_create(&pid, &attr, Function_t, NULL);  
	
	getchar();  
	pthread_attr_destroy(&attr);  
	return 0;  
}

#endif