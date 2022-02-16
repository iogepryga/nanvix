#include <nanvix/const.h>
#include <nanvix/pm.h>



PUBLIC int sys_semctl(int semid, int cmd, int val) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = semtab[semid];

	if (s->state == SEM_IDLE)
		return -1;

	switch (cmd) {
		case GETVAL:
			return s->count;

		case SETVAL:
			s->count = val;

		case IPC_RMID:
			return sem_destroy(semid)

		default:
			return -1;
	}
}
