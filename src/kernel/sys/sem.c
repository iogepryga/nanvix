#include <sys/sem.h>
#include <nanvix/pm.h>
#include <nanvix/const.h>



// Structure de sémaphore
struct sem {
	unsigned state; // Etat de la sémaphore
	unsigned count; // Nombre de permissions de la sémaphore
	unsigned key;   // Clé associée à la sémaphore

	struct process **queue; // Processus en file d'attente
};


// Nombre maximal de sémaphores
#define SEM_MAX 64

// Définition de la première et de la dernière sémaphore (pointeurs de structure)
#define FIRST_SEM ((&semtab[0]))
#define LAST_SEM ((&semtab[SEM_MAX-1]))

// Etats possibles des sémaphores
#define SEM_IDLE 0
#define SEM_ACTIVE 1


// Tableau des structures sémaphores (l'ID d'une sémaphore est sa place dans le tableau : 0 .. SEM_MAX-1)
PUBLIC struct sem semtab[SEM_MAX];

// Nombre de sémaphores actives
PUBLIC unsigned nsems = 0;



// Initialisation des sémaphores du tableau à leur état initial
PUBLIC void sem_init(void) {
	struct sem *s;

	for (s = FIRST_SEM; s <= LAST_SEM; s++) {
		s->state = SEM_IDLE;
		s->count = 0;
		s->key = 0;
		s->queue = NULL;
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
	s->queue = NULL;

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

	if (s->count == 0)
		sleep(s->queue, curr_proc->priority);
	else
		s->count--;

	return semid;
}

// Relâche une sémaphore, et réveille le premier processus en attente
int sem_up(int semid) {
	if (semid < 0 || semid >= SEM_MAX)
		return -1;

	struct sem *s = &semtab[semid];

	if (s->state == SEM_IDLE)
		return -1;

	wakeup(s->queue);
	s->count++;

	return semid;
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
	s->queue = NULL;

	nsems--;

	return 0;
}
