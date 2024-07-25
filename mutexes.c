/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 06:31:28 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/25 06:44:35 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex(t_data_t *data)
{
	int	i;

	if (pthread_mutex_init(&data->lock_printf, NULL))
		return (1);
	i = -1;
	while (++i < data->philo_total)
	{
		if (pthread_mutex_init(&data->lock_forks[i], NULL))
		{
			pthread_mutex_destroy(&data->lock_printf);
			mutex_destroy(data->lock_forks, i);
			return (1);
		}
		if (pthread_mutex_init(&data->lock_thread[i], NULL))
		{
			pthread_mutex_destroy(&data->lock_forks[i]);
			mutex_destroy(data->lock_forks, i + 1);
			mutex_destroy(data->lock_thread, i);
			return (1);
		}
	}
	return (0);
}

void	mutex_destroy(pthread_mutex_t *lock, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&lock[i]);
}
