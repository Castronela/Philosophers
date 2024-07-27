/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 06:29:10 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/27 05:38:37 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	setup_data(t_data_t *data)
{
	if (memalloc_data(data))
		return (error(data, ERROR_MEM_ALLOC, 0));
	if (init_time(&data->time_synch_start))
		return (error(data, ERROR_GETTIMEOFDAY, 2));
	if (init_mutex(data))
		return (error(data, ERROR_MUTEX_INIT, 2));
	return (0);
}

static int	memalloc_data(t_data_t *data)
{
	data->philo = malloc(data->philo_total * sizeof(t_philo_t));
	if (data->philo == NULL)
		return (1);
	data->philo_thread = malloc(data->philo_total * sizeof(pthread_t));
	if (data->philo_thread == NULL)
	{
		free(data->philo);
		return (1);
	}
	if (vars_allocmem(data))
	{
		free(data->philo_thread);
		free(data->philo);
		return (1);
	}
	if (mutex_allocmem(data))
	{
		free(data->thread_status);
		free(data->forks);
		free(data->philo_thread);
		free(data->philo);
		return (1);
	}
	return (0);
}

static int	vars_allocmem(t_data_t *data)
{
	int	i;

	data->forks = malloc(data->philo_total * sizeof(t_fork));
	if (data->forks == NULL)
		return (1);
	data->thread_status = malloc(data->philo_total * sizeof(t_thread));
	if (data->thread_status == NULL)
	{
		free(data->forks);
		return (1);
	}
	i = -1;
	while (++i < data->philo_total)
	{
		data->philo[i].fork_left = &data->forks[i];
		if (i + 1 == data->philo_total)
			data->philo[i].fork_right = &data->forks[0];
		else
			data->philo[i].fork_right = &data->forks[i + 1];
		data->philo[i].thread = &data->thread_status[i];
		data->philo[i].lock_printf = &data->lock_printf;
		data->philo[i].someone_died = &data->someone_died;
	}
	return (0);
}

static int	mutex_allocmem(t_data_t *data)
{
	int	i;

	data->lock_forks = malloc(data->philo_total * sizeof(pthread_mutex_t));
	if (data->lock_forks == NULL)
		return (1);
	data->lock_thread = malloc(data->philo_total * sizeof(pthread_mutex_t));
	if (data->lock_thread == NULL)
	{
		free(data->lock_forks);
		return (1);
	}
	i = -1;
	while (++i < data->philo_total)
	{
		data->philo[i].lock_fork_left = &data->lock_forks[i];
		if (i + 1 == data->philo_total)
			data->philo[i].lock_fork_right = &data->lock_forks[0];
		else
			data->philo[i].lock_fork_right = &data->lock_forks[i + 1];
		data->philo[i].lock_thread = &data->lock_thread[i];
		data->philo[i].lock_printf = &data->lock_printf;
	}
	return (0);
}

void	clean(t_data_t *data)
{
	free(data->lock_thread);
	free(data->thread_status);
	free(data->lock_forks);
	free(data->forks);
	free(data->philo_thread);
	free(data->philo);
}
