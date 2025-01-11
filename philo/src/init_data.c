/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:52:37 by david             #+#    #+#             */
/*   Updated: 2025/01/11 16:24:09 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void		data_init(t_philo_data *data);

static void	data_copy_philo(t_philo_data *data, t_philos *philos, size_t index);

void	data_init(t_philo_data *data)
{
	size_t	index;

	data->time_start_ms += START_DELAY_MS;
	index = -1;
	while (++index < data->philo_count)
		data_copy_philo(data, &data->philos[index], index);
}

static void	data_copy_philo(t_philo_data *data, t_philos *philos, size_t index)
{
	philos->time_start_ms = data->time_start_ms;
	philos->philo_id = index + 1;
	philos->eat_count = data->eat_count;
	philos->time_ms = 0;
	philos->time_die = data->time_die;
	philos->time_eat = data->time_eat;
	philos->time_sleep = data->time_sleep;
	philos->stop_threads = &data->stop_threads;
	philos->fork_left = &data->table_w_forks[index];
	philos->fork_right = &data->table_w_forks[(index + 1) % data->philo_count];
	philos->lock_fork_left = &data->lock_fork[index];
	philos->lock_fork_right = &data->lock_fork[(index + 1) % data->philo_count];
	philos->stop_threads = &data->stop_threads;
	philos->stop_printf = &data->stop_printf;
	philos->lock_threads = &data->lock_threads;
	philos->lock_printf = &data->lock_printf;
}
