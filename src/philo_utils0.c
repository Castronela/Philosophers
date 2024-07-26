/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 06:41:19 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/25 06:44:11 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_iteration_latency(t_philo_t *philo)
{
	uint64_t	time_current;

	if (init_time(&time_current))
	{
		put_stdout(philo, ERROR_GETTIMEOFDAY, 1);
		set_thread(philo, STOPPED);
		return ;
	}
	if (time_current >= philo->time_synch_start)
	{
		philo->time_synch_start += 1000;
		philo->time_event++;
		philo->time_eat_last++;
		philo->time++;
	}
}

void	get_forks(t_philo_t *philo)
{
	while (philo->fork_count < 2)
	{
		pthread_mutex_lock(philo->lock_fork_left);
		if (*philo->fork_left == FORK)
		{
			*philo->fork_left = NO_FORK;
			philo->fork_count += 1;
			put_stdout(philo, "has taken a fork", 0);
		}
		pthread_mutex_unlock(philo->lock_fork_left);
		pthread_mutex_lock(philo->lock_fork_right);
		if (*philo->fork_right == FORK)
		{
			*philo->fork_right = NO_FORK;
			philo->fork_count += 1;
			put_stdout(philo, "has taken a fork", 0);
		}
		pthread_mutex_unlock(philo->lock_fork_right);
	}
}

void	reset_forks(t_philo_t *philo)
{
	if (philo->fork_count == 2)
	{
		pthread_mutex_lock(philo->lock_fork_left);
		*philo->fork_left = FORK;
		pthread_mutex_unlock(philo->lock_fork_left);
		pthread_mutex_lock(philo->lock_fork_right);
		*philo->fork_right = FORK;
		pthread_mutex_unlock(philo->lock_fork_right);
		philo->fork_count = 0;
		philo->eat_count--;
	}
}

void	philo_synch(t_philo_t *philo)
{
	uint64_t	time_current;

	if (init_time(&time_current))
	{
		put_stdout(philo, ERROR_GETTIMEOFDAY, 1);
		set_thread(philo, STOPPED);
		return ;
	}
	while (time_current < philo->time_synch_start)
	{
		if (init_time(&time_current))
		{
			put_stdout(philo, ERROR_GETTIMEOFDAY, 1);
			set_thread(philo, STOPPED);
			return ;
		}
		usleep(100);
	}
	philo->time_synch_start += 1000;
}

void	put_stdout(t_philo_t *philo, char *action, int error)
{
	pthread_mutex_lock(philo->lock_printf);
	if (error)
		printf("%s", ERROR_GETTIMEOFDAY);
	else if (*philo->someone_died == false)
		printf("%d %d %s\n", philo->time, philo->philo_id, action);
	if (philo->status == DEAD)
		*philo->someone_died = true;
	pthread_mutex_unlock(philo->lock_printf);
}
