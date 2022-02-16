#include <nanvix/const.h>
#include <nanvix/syscall.h>
//#include <nanvix/pm.h>
//#include <sys/sem.h>



// Effectue une opération sur une sémaphore
// Si op est positif, on relâche la sémaphore
// Si op est négatif, on demande à prendre la sémaphore
// Sinon (op == 0), on retourne -1 (erreur)
PUBLIC int sys_semop(int semid, int op) {
	if (op > 0)
		return sem_up(semid);
	else if (op < 0)
		return sem_down(semid);
	else
		return -1;
}
