#include <nanvix/const.h>
#include <sys/sem.h>
//#include <nanvix/syscall.h>
//#include <nanvix/pm.h>
//#include <sys/sem.h>



// Effectue une commande sur une sémaphore
// Si cmd vaut GETVAL, on retourne le nombre de permissions disponibles de la sémaphore
// Si cmd vaut SETVAL, on définit le nombre de permissions disponibles de la sémaphore à val
// Si cmd vaut IPC_RMID, on demande la destruction de la sémaphore
PUBLIC int sys_semctl(int semid, int cmd, int val) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = &semtab[semid];

	if (s->state == SEM_IDLE)
		return -1;

	switch (cmd) {
		case GETVAL:
			return s->count;

		case SETVAL:
			s->count = val;
			return val;

		case IPC_RMID:
			return sem_destroy(semid);

		default:
			return -1;
	}
}
