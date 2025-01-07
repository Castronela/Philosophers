/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:04:41 by david             #+#    #+#             */
/*   Updated: 2025/01/07 18:53:36 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		update_time(t_philos *philo);
int64_t	get_timeofday_ms(const bool safe, t_philos *philo);
int		print_safe(t_philos *philo, const bool is_philo_state, const char *msg,
			const int exit_code);

int64_t	get_timeofday_ms(const bool safe, t_philos *philo)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		if (safe)
		{
			philo->status = INT_FAIL;
			print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_TIMEOFDAY, 0);
		}
		else
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_TIMEOFDAY);
		return (0);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	update_time(t_philos *philo)
{
	uint64_t	current_time_ms;

	current_time_ms = get_timeofday_ms(1, philo);
	if (!current_time_ms)
		return (EXIT_FAILURE);
	if (current_time_ms > philo->time_start_ms)
	{
		philo->time_ms += current_time_ms - philo->time_start_ms;
		philo->time_start_ms = current_time_ms;
	}
	return (EXIT_SUCCESS);
}

int	print_safe(t_philos *philo, const bool is_philo_state, const char *msg,
		const int exit_code)
{
	if (pthread_mutex_lock(philo->lock_printf))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK),
			EXIT_FAILURE);
	if (is_philo_state && !(*philo->stop_threads))
	{
		if (printf("%d %d %s\n", philo->time_ms, philo->philo_id, msg) < 0)
			return (EXIT_FAILURE);
	}
	else
	{
		if (printf("%s\n", msg) < 0)
			return (EXIT_FAILURE);
		if (pthread_mutex_unlock(philo->lock_printf))
			return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK),
				EXIT_FAILURE);
		return (exit_code);
	}
	if (philo->status == DEAD || philo->status == INT_FAIL)
		*philo->stop_threads = true;
	else if (*philo->stop_threads && philo->status == RUNNING)
		philo->status = STOPPED;
	if (pthread_mutex_unlock(philo->lock_printf))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK),
			EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
