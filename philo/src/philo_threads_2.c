/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:32:50 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/07 18:53:42 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	apply_sleep(t_philos *philo, const unsigned int time_ms);
int	forks_pickup(t_philos *philo);
int	forks_putback(t_philos *philo);
int	print_action(t_philos *philo, const int action);
int	check_stopped_thread(t_philos *philo);

int	apply_sleep(t_philos *philo, const unsigned int time_ms)
{
	int64_t	end_time;
	int64_t	current_time;

	end_time = get_timeofday_ms(1, philo);
	if (!end_time)
		return (EXIT_FAILURE);
	current_time = end_time;
	end_time += time_ms;
	while (current_time < end_time)
	{
		current_time = get_timeofday_ms(1, philo);
		if (!current_time)
			return (EXIT_FAILURE);
		check_stopped_thread(philo);
		if (philo->status != RUNNING)
			break ;
		usleep(100);
	}
	return (EXIT_SUCCESS);
}

/*
1 - thinking
2 - eating
3 - sleeping
4 - dead
*/
int	print_action(t_philos *philo, const int action)
{
	if (update_time(philo))
		return (EXIT_FAILURE);
	if (action == 1 && print_safe(philo, 1, "is thinking", 0))
		return (EXIT_FAILURE);
	else if (action == 2)
	{
		if (print_safe(philo, 1, "has taken a fork", 0) || print_safe(philo, 1,
				"has taken a fork", 0) || print_safe(philo, 1, "is eating", 0))
			return (EXIT_FAILURE);
	}
	else if (action == 3 && print_safe(philo, 1, "is sleeping", 0))
		return (EXIT_FAILURE);
	else if (action == 4 && print_safe(philo, 1, "died", 0))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	forks_pickup(t_philos *philo)
{
	if (pthread_mutex_lock(philo->lock_fork_right))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK, 1));
	if (*philo->fork_right)
	{
		*philo->fork_right = 0;
		philo->fork_count++;
	}
	if (pthread_mutex_unlock(philo->lock_fork_right))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK, 1));
	if (pthread_mutex_lock(philo->lock_fork_left))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK, 1));
	if (*philo->fork_left)
	{
		*philo->fork_left = 0;
		philo->fork_count++;
	}
	if (pthread_mutex_unlock(philo->lock_fork_left))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK, 1));
	return (EXIT_SUCCESS);
}

int	forks_putback(t_philos *philo)
{
	if (pthread_mutex_lock(philo->lock_fork_left))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK, 1));
	if (!(*philo->fork_left))
	{
		*philo->fork_left = 1;
		philo->fork_count--;
	}
	if (pthread_mutex_unlock(philo->lock_fork_left))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK, 1));
	if (pthread_mutex_lock(philo->lock_fork_right))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK, 1));
	if (!(*philo->fork_right))
	{
		*philo->fork_right = 1;
		philo->fork_count--;
	}
	if (pthread_mutex_unlock(philo->lock_fork_right))
		return (print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK, 1));
	return (EXIT_SUCCESS);
}

int	check_stopped_thread(t_philos *philo)
{
	if (update_time(philo))
		return (EXIT_FAILURE);
	if (philo->time_die <= philo->time_ms - philo->time_last_eat)
	{
		philo->status = DEAD;
		if (print_action(philo, 4))
			return (EXIT_FAILURE);
	}
	else if (!philo->eat_count)
		philo->status = EXITED;
	if (pthread_mutex_lock(philo->lock_printf))
		return (EXIT_FAILURE);
	if (*philo->stop_threads && philo->status == RUNNING)
		philo->status = STOPPED;
	if (pthread_mutex_unlock(philo->lock_printf))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
