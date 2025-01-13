/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:59:55 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/13 16:57:37 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int64_t	get_timeofday_ms(void);
int		update_time(t_philos *philo, uint64_t current_time_ms);
int		apply_sleep(t_philos *philo, const unsigned int time_ms);

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
		if (check_stopped_thread(philo, current_time))
			return (EXIT_FAILURE);
		if (philo->status != RUNNING)
			break ;
	}
	return (EXIT_SUCCESS);
}
