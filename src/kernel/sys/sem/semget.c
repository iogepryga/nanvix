#include <nanvix/const.h>
#include <nanvix/syscall.h>
//#include <nanvix/pm.h>
//#include <sys/sem.h>



// Retourne l'identifiant d'une sémaphore en spécifiant une clé
// Si aucune sémaphore n'est associée à la clé, alors on crée une sémaphore et on l'associe à la clé,
// puis on retourne l'identifiant de la sémaphore nouvellement créée
// Si une sémaphore comportant cette clé existe, on retourne simplement son identifiant
// Si aucune semaphore n'est associée à cette clé, et qu'il n'y a plus de sémaphores disponibles, retourne -1 (erreur)
PUBLIC int sys_semget(unsigned key) {
	struct sem *s;
	int i, first_idle = -1;

	// On parcourt le tableau de sémaphores jusqu'à en trouver une (ou pas) associée à la clé
	// Par la même occasion, on relève l'identifiant de la première sémaphore disponible
	for (s = FIRST_SEM, i = 0; s <= LAST_SEM; s++, i++) {
		if (s->state == SEM_IDLE && first_idle == -1)
			first_idle = i;
		if (s->key == key)
			break;
	}

	// Si aucune sémaphore ne correspond à la clé, on active une sémaphore
	if (i == SEM_MAX) {
		// S'il n'y a plus de sémaphores disponibles, erreur (-1)
		if (first_idle == -1)
			return -1;
		
		// S'il reste une sémaphore disponible, on l'active et on retourne son identifiant
		else
			return sem_create(first_idle, key, 1);
	}
	
	// Si une sémaphore contenant la clé demandée a été trouvée, on retourne son identifiant
	else
		return i;
}
