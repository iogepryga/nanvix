#include <nanvix/const.h>
#include <nanvix/pm.h>



PUBLIC int sys_semget(unsigned key) {
	struct sem *s;
	int i, first_idle = -1;

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
