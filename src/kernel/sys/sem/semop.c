#include <nanvix/const.h>
#include <nanvix/pm.h>



PUBLIC int sys_semop(int semid, int op) {
	if (op > 0)
		return sem_up(semid);
	else if (op < 0)
		return sem_down(semid);
	else
		return -1;
}
