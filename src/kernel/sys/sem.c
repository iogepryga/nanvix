#include <sys/sem.h>
#include <nanvix/const.h>


struct sem {
	unsigned state;
	unsigned count;

	struct process **queue;
};


#define SEM_MAX 64


PUBLIC struct sem semtab[SEM_MAX];
// Table des semaphores est statique

PUBLIC unsigned nsems = 0;



PUBLIC void sem_init(void) {

}
