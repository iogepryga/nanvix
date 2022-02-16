#include <sys/sem.h>
#include <nanvix/const.h>


// Structure de sémaphore
struct sem {
	unsigned state; // Etat
	unsigned count; // Nombre de places disponibles
	unsigned key;   // Clé associée à la sémaphore

	struct process **queue; // Processus en file d'attente
};


// Nombre maximal de sémaphores
#define SEM_MAX 64

// Définition de la première et de la dernière sémaphore (pointeurs de structure)
#define FIRST_SEM ((&semtab[0]))
#define LAST_SEM ((&semtab[SEM_MAX - 1]))

// Etats possibles des sémaphores
#define SEM_IDLE 0
#define SEM_ACTIVE 1


// Tableau des structures sémaphores (l'ID d'une sémaphore est sa place dans le tableau)
PUBLIC struct sem semtab[SEM_MAX];

// Nombre de sémaphores actives
PUBLIC unsigned nsems = 0;



// Initialisation des sémaphores du tableau à leur état initial
PUBLIC void sem_init(void) {
	struct sem *s

	for (s = FIRST_SEM; s <= LAST_SEM; s++) {
		s->state = SEM_IDLE;
		s->count = 0;
		s->key = 0;
		s->queue = NULL;
	}
}


int sem_create(int semid, unsigned key, int n) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = semtab[semid];

	if (s->state == SEM_ACTIVE)
		return -1;

	s->state = SEM_ACTIVE
	s->count = n;
	s->key = key;
	s->queue = NULL;

	return semid;
}

int sem_down(int semid) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = semtab[i];

	if (s->state == SEM_IDLE)
		return -1;

	if (s->count == 0)
		sleep(s->queue, curr_proc->priority);
	else
		s->count--;

	return semid;
}

int sem_up(int semid) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = semtab[i];

	if (s->state == SEM_IDLE)
		return -1;

	wakeup(s->queue);
	s->count++;

	return semid;
}

int destroy(int semid) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = semtab[i];

	if (s->state == SEM_IDLE)
		return -1;

	s->state = SEM_IDLE;
	s->count = 0;
	s->key = 0;
	s->queue = NULL;

	return semid;
}
