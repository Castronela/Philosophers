/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:04:41 by david             #+#    #+#             */
/*   Updated: 2025/01/31 02:58:05 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int64_t		get_timeofday_ms(void);
int			update_time(t_philos *philo, uint64_t current_time_ms);
static void	print_action(t_philos *philo, const int action);
int			print_safe(t_philos *philo, const int action);

int64_t	get_timeofday_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		printf("%s\n", ERRMSG_INTERNAL ERRMSG_TIMEOFDAY);
		return (0);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	update_time(t_philos *philo, uint64_t current_time_ms)
{
	if (!current_time_ms)
		current_time_ms = get_timeofday_ms();
	if (!current_time_ms)
		return (EXIT_FAILURE);
	if (current_time_ms > philo->time_start_ms)
	{
		philo->time_ms += current_time_ms - philo->time_start_ms;
		philo->time_start_ms = current_time_ms;
	}
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
		printf("%d %d is thinking\n", philo->time_ms, philo->philo_id);
	else if (action == 2)
	{
		printf("%d %d has taken a fork\n", philo->time_ms, philo->philo_id);
		printf("%d %d has taken a fork\n", philo->time_ms, philo->philo_id);
		printf("%d %d is eating\n", philo->time_ms, philo->philo_id);
	}
	else if (action == 3)
		printf("%d %d is sleeping\n", philo->time_ms, philo->philo_id);
	if (action == 4)
		printf("%d %d died\n", philo->time_ms, philo->philo_id);
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
		if (is_stopped(philo, RUNNING))
			*philo->stop_printf = true;
	}
	if (pthread_mutex_unlock(philo->lock_printf))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK),
			EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
