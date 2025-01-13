/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 19:32:59 by david             #+#    #+#             */
/*   Updated: 2025/01/13 15:41:47 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void		*philo_thread(void *philo_void);

static int	philo_init(t_philos *philo);
static int	philo_eat(t_philos *philo);
static int	philo_sleep(t_philos *philo);
static int	apply_start_delay(t_philos *philo);

void	*philo_thread(void *philo_void)
{
	t_philos	*philo;

	philo = philo_void;
	if (philo_init(philo))
		philo->status = INT_FAIL;
	while (philo->status == RUNNING)
	{
		if (philo_eat(philo))
			thread_stop(philo, INT_FAIL);
		if (philo->status != RUNNING)
			break ;
		if (philo_sleep(philo))
			thread_stop(philo, INT_FAIL);
		usleep(USLEEP_VALUE);
	}
	return (NULL);
}

static int	philo_eat(t_philos *philo)
{
	if (philo->fork_count < 2)
	{
		if (forks_pickup(philo))
			return (EXIT_FAILURE);
		if (philo->fork_count < 2 && print_safe(philo, 1))
			return (EXIT_FAILURE);
	}
	while (philo->fork_count < 2)
	{
		usleep(USLEEP_VALUE);
		if (check_stopped_thread(philo, 0))
			return (EXIT_FAILURE);
		if (philo->status != RUNNING)
			return (EXIT_SUCCESS);
		if (forks_pickup(philo))
			return (EXIT_FAILURE);
	}
	if (print_safe(philo, 2))
		return (EXIT_FAILURE);
	philo->time_last_eat = philo->time_ms;
	if (apply_sleep(philo, philo->time_eat))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	philo_sleep(t_philos *philo)
{
	if (forks_putback(philo))
		return (EXIT_FAILURE);
	if (print_safe(philo, 3))
		return (EXIT_FAILURE);
	philo->eat_count--;
	if (apply_sleep(philo, philo->time_sleep))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	apply_start_delay(t_philos *philo)
{
	uint64_t	current_time_ms;

	current_time_ms = get_timeofday_ms();
	if (!current_time_ms)
		return (EXIT_FAILURE);
	while (current_time_ms < philo->time_start_ms)
	{
		usleep(USLEEP_VALUE);
		current_time_ms = get_timeofday_ms();
		if (!current_time_ms)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	philo_init(t_philos *philo)
{
	philo->status = RUNNING;
	philo->time_ms = 0;
	philo->time_last_eat = 0;
	philo->fork_count = 0;
	if (!(philo->philo_id & 1))
	{
		if (forks_pickup(philo))
			return (EXIT_FAILURE);
	}
	else
	{
		philo->time_start_ms += philo->time_eat;
		philo->time_ms = philo->time_eat;
		philo->time_last_eat = philo->time_eat;
	}
	if (apply_start_delay(philo))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
