/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 20:32:12 by david             #+#    #+#             */
/*   Updated: 2025/01/31 04:00:04 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			data_init(t_philo_data *data);
void		data_free(t_philo_data *data, const int destroy_mutex);

static int	mutex_init_all(t_philo_data *data);
static int	mutex_init_philos(t_philo_data *data, const size_t size);
static void	mutex_destroy_philos(t_philo_data *data, int index);

int	data_init(t_philo_data *data)
{
	data->time_start_ms = get_timeofday_ms();
	if (!data->time_start_ms)
		return (EXIT_FAILURE);
	data->philos = malloc(sizeof(*data->philos) * data->philo_count);
	if (!data->philos)
	{
		printf("%s\n", ERRMSG_INTERNAL ERRMSG_MALLOC);
		return (EXIT_FAILURE);
	}
	if (mutex_init_all(data))
		return (EXIT_FAILURE);
	philos_init(data, data->philos, 0);
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
	if (mutex_init_philos(data, data->philo_count))
	{
		if (pthread_mutex_destroy(&data->lock_printf))
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
		data_free(data, 0);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	mutex_init_philos(t_philo_data *data, const size_t size)
{
	size_t	index;

	index = 0;
	while (index < size)
	{
		if (pthread_mutex_init(&data->philos[index].lock_fork_right, NULL))
		{
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_INIT);
			mutex_destroy_philos(data, index);
			return (EXIT_FAILURE);
		}
		if (pthread_mutex_init(&data->philos[index].lock_status, NULL))
		{
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_INIT);
			if (pthread_mutex_destroy(&data->philos[index].lock_fork_right))
				printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
			mutex_destroy_philos(data, index);
			return (EXIT_FAILURE);
		}
		index++;
	}
	return (EXIT_SUCCESS);
}

void	mutex_destroy_philos(t_philo_data *data, int index)
{
	while (--index >= 0)
	{
		if (pthread_mutex_destroy(&data->philos[index].lock_fork_right))
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
		if (pthread_mutex_destroy(&data->philos[index].lock_status))
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
	}
}

void	data_free(t_philo_data *data, const int destroy_mutex)
{
	if (destroy_mutex)
	{
		mutex_destroy_philos(data, data->philo_count);
		if (pthread_mutex_destroy(&data->lock_printf))
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
	}
	if (data->philos)
		free(data->philos);
}
