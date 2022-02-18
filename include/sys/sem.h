/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEM_H_
#define SEM_H_

	/**
	 * @brief Command values for semaphores.
	 */
	/**@{*/
	#define GETVAL   0 /**< Returns the value of a semaphore. */
	#define SETVAL   1 /**< Sets the value of a semaphore.    */
	#define IPC_RMID 3 /**< Destroys a semaphore.            */
	/**@}*/

    // Nombre maximal de sémaphores
    #define SEM_MAX 64

    // Définition de la première et de la dernière sémaphore (pointeurs de structure)
    #define FIRST_SEM ((&semtab[0]))
    #define LAST_SEM ((&semtab[SEM_MAX-1]))

    // Etats possibles des sémaphores
    #define SEM_IDLE 0
    #define SEM_ACTIVE 1


    #ifndef _ASM_FILE_

    // Structure de sémaphore
    struct sem {
        unsigned state; // Etat de la sémaphore
        unsigned count; // Nombre de permissions de la sémaphore
        unsigned key;   // Clé associée à la sémaphore

        struct process **queue; // Processus en file d'attente
    };

    #endif /* _ASM_FILE_ */



	/* Forward definitions. */
	extern int semget(unsigned);
	extern int semctl(int, int, int);
	extern int semop(int, int);

#endif /* SEM_H_ */
