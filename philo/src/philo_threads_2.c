/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:32:50 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/13 16:00:45 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			check_stopped_thread(t_philos *philo, uint64_t current_time_ms);
bool		thread_stop(t_philos *philo, const t_state change_status);
int			print_safe(t_philos *philo, const int action);

static void	print_action(t_philos *philo, const int action);

int	check_stopped_thread(t_philos *philo, uint64_t current_time_ms)
{
	if (update_time(philo, current_time_ms))
		return (EXIT_FAILURE);
	if (philo->time_die <= philo->time_ms - philo->time_last_eat)
	{
		thread_stop(philo, DEAD);
		if (print_safe(philo, 4))
			return (EXIT_FAILURE);
	}
	else if (!philo->eat_count)
		thread_stop(philo, EXITED);
	if (philo->status == RUNNING && thread_stop(philo, 0))
		philo->status = STOPPED;
	return (EXIT_SUCCESS);
}

bool	thread_stop(t_philos *philo, const t_state change_status)
{
	bool	ret_value;

	ret_value = false;
	if (pthread_mutex_lock(philo->lock_threads))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (change_status)
	{
		*philo->stop_threads = change_status;
		philo->status = change_status;
	}
	if (*philo->stop_threads != RUNNING)
		ret_value = true;
	if (pthread_mutex_unlock(philo->lock_threads))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	return (ret_value);
}

int	print_safe(t_philos *philo, const int action)
{
	if (pthread_mutex_lock(philo->lock_printf))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK),
			EXIT_FAILURE);
	if (!(*philo->stop_printf))
	{
		if (update_time(philo, 0))
			return (EXIT_FAILURE);
		print_action(philo, action);
		if (philo->status != RUNNING && philo->status != EXITED)
			*philo->stop_printf = true;
	}
	if (pthread_mutex_unlock(philo->lock_printf))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK),
			EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
1 - thinking
2 - eating
3 - sleeping
4 - dead
*/
static void	print_action(t_philos *philo, const int action)
{
	if (action == 1)
		printf("%d %d %s\n", philo->time_ms, philo->philo_id, MSG_THINK);
	else if (action == 2)
	{
		printf("%d %d %s\n%d %d %s\n%d %d %s\n", philo->time_ms,
			philo->philo_id, MSG_FORK, philo->time_ms, philo->philo_id,
			MSG_FORK, philo->time_ms, philo->philo_id, MSG_EAT);
	}
	else if (action == 3)
		printf("%d %d %s\n", philo->time_ms, philo->philo_id, MSG_SLEEP);
	if (action == 4)
		printf("%d %d %s\n", philo->time_ms, philo->philo_id, MSG_DEAD);
}
