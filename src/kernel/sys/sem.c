#include <sys/sem.h>
#include <nanvix/pm.h>
#include <nanvix/const.h>



// Nombre de sémaphores actives
PUBLIC unsigned nsems = 0;



// Initialisation des sémaphores du tableau à leur état initial
PUBLIC void sem_init(void) {
	struct sem *s;

	for (s = FIRST_SEM; s <= LAST_SEM; s++) {
		s->state = SEM_IDLE;
		s->count = 0;
		s->key = 0;
		s->waiting_queue = NULL;
	}
}



// Permet de créer une sémaphore, en spécifiant son identifiant, une clé et le nombre de permissions
int sem_create(int semid, unsigned key, int n) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = &semtab[semid];

	if (s->state == SEM_ACTIVE)
		return -1;

	s->state = SEM_ACTIVE;
	s->count = n;
	s->key = key;
	s->waiting_queue = NULL;

	nsems++;

	return semid;
}

// Demande une permission à la sémaphore, et bloque le processus s'il n'y en a plus de disponible
int sem_down(int semid) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = &semtab[semid];

	if (s->state == SEM_IDLE)
		return -1;

	while (s->count <= 0)
		sleep(&(s->waiting_queue), curr_proc->priority);
	
	s->count--;

	return 0;
}

// Relâche une sémaphore, et réveille le premier processus en attente
int sem_up(int semid) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = &semtab[semid];

	if (s->state == SEM_IDLE)
		return -1;

	wakeup(&(s->waiting_queue));
	s->count++;

	return 0;
}

// Détruit une sémaphore, et la rend disponible pour une future initialisation
// Retourne 0 si la destruction s'est bien déroulée, -1 sinon (erreur)
int sem_destroy(int semid) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = &semtab[semid];

	if (s->state == SEM_IDLE)
		return -1;

	s->state = SEM_IDLE;
	s->count = 0;
	s->key = 0;
	s->waiting_queue = NULL;

	nsems--;

	return 0;
}
