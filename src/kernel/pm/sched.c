/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/pm.h>
#include <signal.h>

/**
 * @brief Schedules a process to execution.
 * 
 * @param proc Process to be scheduled.
 */
PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	yield();
}

/**
 * @brief Resumes a process.
 * 
 * @param proc Process to be resumed.
 * 
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{	
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}

/**
 * @brief Yields the processor.
 */
PUBLIC void yield(void)
{
	struct process *p;    /* Working process.     */
	struct process *next; /* Next process to run. */

	/* Re-schedule process for execution. */
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	/* Remember this process. */
	last_proc = curr_proc;

	/* Check alarm. */
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip invalid processes. */
		if (!IS_VALID(p))
			continue;
		
		/* Alarm has expired. */
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}

	/* Choose a process to run next. */
	next = IDLE;
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY)
			continue;
		

		/* ===== IL FAUT CHOISIR UNE DES STRATEGIES DE SCHEDULING (DECOMMENTER UN DES IF) ===== */

		// Process with higher waiting time found.
//		if (p->counter > next->counter)

//		if (p->priority + p->nice < next->priority + next->nice || (p->priority + p->nice == next->priority + next->nice && p->counter > next->counter))

		/* On vérifie que le processus p a une priorité statique (priority) plus petite (+ importante) que next, si c'est le cas, on le sélectionne
		 * S'ils ont la même priorité statique, on vérifie leurs priorités utilisateur (nice), et on sélectionne p si sa priorité est plus petite (+ importante)
		 * S'ils ont les mêmes priorités statique et utilisateur, on vérifie leurs priorités dynamique (counter), et on sélectionne p si sa priorité est plus grande (+ importante)
		 */
//		if (p->priority < next->priority || (p->priority == next->priority && p->nice < next->nice) || (p->priority == next->priority && p->nice == next->nice && p->counter > next->counter))

		/* On somme les priorités statique, utilisateur et dynamique (on la soustrait) des processus et on prend celui qui a la plus faible */
		if (p->priority + p->nice - p->counter < next->priority + next->nice - next->counter)
		{
			next->counter++;
			next = p;
		}


			
		/*
		 * Increment waiting
		 * time of process.
		 */
		else
			p->counter++;
	}
	
	/* Switch to next process. */
	next->priority = PRIO_USER;
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM;
	if (curr_proc != next)
		switch_to(next);
}
