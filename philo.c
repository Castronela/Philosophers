/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 04:36:18 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/25 06:46:42 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_thread(void *arg)
{
	t_philo_t	*philo;

	philo = (t_philo_t *)arg;
	philo_synch(philo);
	while (thread_check(philo) == false)
	{
		philo_iteration_latency(philo);
		if ((philo->status == THINK && philo->time_event >= philo->time_think)
			|| (philo->status == START && philo->time >= philo->time_start))
			set_status(philo, EAT);
		if (philo->status != EAT)
			reset_forks(philo);
		if (philo->status == EAT && philo->time_event >= philo->time_eat)
			set_status(philo, SLEEP);
		if (philo->eat_count == 0)
			set_thread(philo, ENDED);
		if (philo->status == SLEEP && philo->time_event >= philo->time_sleep)
			set_status(philo, THINK);
		if (philo->time_eat_last >= philo->time_death)
			set_status(philo, DEAD);
		usleep(50);
	}
	return (NULL);
}
