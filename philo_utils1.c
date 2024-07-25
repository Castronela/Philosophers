/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 06:41:34 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/25 06:46:54 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_thread(t_philo_t *philo, t_thread thread)
{
	pthread_mutex_lock(philo->lock_thread);
	*philo->thread = thread;
	pthread_mutex_unlock(philo->lock_thread);
}

bool	thread_check(t_philo_t *philo)
{
	bool	ret_value;

	ret_value = false;
	pthread_mutex_lock(philo->lock_thread);
	if (*philo->thread == STOPPED)
		ret_value = true;
	pthread_mutex_unlock(philo->lock_thread);
	return (ret_value);
}

void	set_status(t_philo_t *philo, t_status_t status)
{
	if (status == EAT && philo->time_eat_last < philo->time_death
		&& philo->eat_count != 0)
	{
		get_forks(philo);
		put_stdout(philo, "is eating", 0);
		philo->time_eat_last = 0;
	}
	else if (status == SLEEP && philo->time_eat_last < philo->time_death
		&& philo->time_sleep > 0)
		put_stdout(philo, "is sleeping", 0);
	else if (status == THINK && philo->time_eat_last < philo->time_death
		&& philo->time_think > 0)
		put_stdout(philo, "is thinking", 0);
	else if (status == DEAD)
	{
		put_stdout(philo, "died", 0);
		set_thread(philo, STOPPED);
	}
	philo->status = status;
	philo->time_event = 0;
}
