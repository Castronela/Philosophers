/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 20:32:12 by david             #+#    #+#             */
/*   Updated: 2025/01/12 15:27:49 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	data_alloc(t_philo_data *data);
void mutex_destroy_arr(pthread_mutex_t **mutex, int index);

static int	alloc_mem(t_philo_data *data);
static int	mutex_init_all(t_philo_data *data);
static int mutex_init_arr(pthread_mutex_t **mutex, const size_t size);

int	data_alloc(t_philo_data *data)
{
	data->time_start_ms = get_timeofday_ms();
	if (!data->time_start_ms)
		return (EXIT_FAILURE);
	if (alloc_mem(data))
		return (EXIT_FAILURE);
	if (mutex_init_all(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	alloc_mem(t_philo_data *data)
{
	size_t	index;

	data->table_w_forks = malloc(sizeof(*data->table_w_forks)
			* data->philo_count);
	data->lock_fork = malloc(sizeof(*data->lock_fork) * data->philo_count);
	data->philos = malloc(sizeof(*data->philos) * data->philo_count);
	data->stop_threads = malloc(sizeof(*data->stop_threads) * data->philo_count);
	data->lock_threads = malloc(sizeof(*data->lock_threads) * data->philo_count);
	if (!data->lock_fork || !data->table_w_forks || !data->philos || 
	!data->stop_threads || !data->lock_threads)
	{
		data_free(data, 0);
		printf("%s\n", ERRMSG_INTERNAL ERRMSG_MALLOC);
		return (EXIT_FAILURE);
	}
	index = -1;
	while (++index < data->philo_count)
	{
		data->table_w_forks[index] = 1;
		data->stop_threads[index] = RUNNING;
	}
	return (EXIT_SUCCESS);
}

static int	mutex_init_all(t_philo_data *data)
{
	if (pthread_mutex_init(&data->lock_printf, NULL))
	{
		printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_INIT);
		data_free(data, 0);
		return (EXIT_FAILURE);
	}
	if (mutex_init_arr(&data->lock_fork, data->philo_count) || 
		mutex_init_arr(&data->lock_threads, data->philo_count))
	{
		data_free(data, 1);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int mutex_init_arr(pthread_mutex_t **mutex, const size_t size)
{
	size_t index;

	index = 0;
	while (index < size)
	{
		if (pthread_mutex_init(&(*mutex)[index], NULL))
		{
			mutex_destroy_arr(mutex, index);
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_INIT);
			return (EXIT_FAILURE);
		}
		index++;
	}
	return (EXIT_SUCCESS);
}

void mutex_destroy_arr(pthread_mutex_t **mutex, int index)
{
	if (!(*mutex))
		return ;
	while (--index >= 0)
	{
		if (pthread_mutex_destroy(&(*mutex)[index]))
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
	}
	*mutex = NULL;
}
