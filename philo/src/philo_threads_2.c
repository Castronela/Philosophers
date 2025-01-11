/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:32:50 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/11 20:16:32 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	apply_sleep(t_philos *philo, const unsigned int time_ms);
int	forks_pickup(t_philos *philo);
int	forks_putback(t_philos *philo);
int	print_action(t_philos *philo, const int action);
int	check_stopped_thread(t_philos *philo);
bool is_stopped(t_philos *philo, const bool change_status);

int	apply_sleep(t_philos *philo, const unsigned int time_ms)
{
	int64_t	end_time;
	int64_t	current_time;

	end_time = get_timeofday_ms();
	if (!end_time)
		return (EXIT_FAILURE);
	current_time = end_time;
	end_time += time_ms;
	while (current_time < end_time)
	{
		usleep(USLEEP_VALUE);
		current_time = get_timeofday_ms();
		if (!current_time)
			return (EXIT_FAILURE);
		if (check_stopped_thread(philo))
			return (EXIT_FAILURE);
		if (philo->status != RUNNING)
			break ;
	}
	return (EXIT_SUCCESS);
}

int	check_stopped_thread(t_philos *philo)
{
	if (update_time(philo))
		return (EXIT_FAILURE);
	if (philo->time_die <= philo->time_ms - philo->time_last_eat)
	{
		philo->status = DEAD;
		is_stopped(philo, 1);
		if (print_safe(philo, 4))
			return (EXIT_FAILURE);
	}
	else if (!philo->eat_count)
		philo->status = EXITED;
	if (philo->status == RUNNING && is_stopped(philo, 0))
		philo->status = STOPPED;
	else if (philo->status == INT_FAIL)
		is_stopped(philo, 1);
	return (EXIT_SUCCESS);
}

bool is_stopped(t_philos *philo, const bool change_status)
{
	bool ret_value;

	if (pthread_mutex_lock(philo->lock_threads))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (change_status)
		*philo->stop_threads = change_status;
	ret_value = *philo->stop_threads;
	if (pthread_mutex_unlock(philo->lock_threads))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	return (ret_value);
}


int	forks_pickup(t_philos *philo)
{
	if (pthread_mutex_lock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (*philo->fork_right)
	{
		*philo->fork_right = 0;
		philo->fork_count++;
	}
	if (pthread_mutex_unlock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	if (pthread_mutex_lock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (*philo->fork_left)
	{
		*philo->fork_left = 0;
		philo->fork_count++;
	}
	if (pthread_mutex_unlock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	return (EXIT_SUCCESS);
}

int	forks_putback(t_philos *philo)
{
	if (pthread_mutex_lock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (!(*philo->fork_left))
	{
		*philo->fork_left = 1;
		philo->fork_count--;
	}
	if (pthread_mutex_unlock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	if (pthread_mutex_lock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (!(*philo->fork_right))
	{
		*philo->fork_right = 1;
		philo->fork_count--;
	}
	if (pthread_mutex_unlock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	return (EXIT_SUCCESS);
}
