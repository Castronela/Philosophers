/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:52:37 by david             #+#    #+#             */
/*   Updated: 2025/01/31 03:30:49 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philos_init(t_philo_data *data, t_philos *philos, size_t index);

void	philos_init(t_philo_data *data, t_philos *philos, size_t index)
{
	philos->time_start_ms = data->time_start_ms + START_DELAY_MS;
	philos->status = RUNNING;
	philos->philo_id = index + 1;
	philos->eat_count = data->eat_count;
	philos->time_ms = 0;
	philos->time_die = data->time_die;
	philos->time_eat = data->time_eat;
	philos->time_sleep = data->time_sleep;
	philos->fork_left = &data->philos[(index + data->philo_count - 1)
		% data->philo_count].fork_right;
	philos->fork_right = 1;
	philos->lock_fork_left = &data->philos[(index + data->philo_count - 1)
		% data->philo_count].lock_fork_right;
	philos->stop_printf = &data->stop_printf;
	philos->lock_printf = &data->lock_printf;
	index++;
	if (index < data->philo_count)
		philos_init(data, &data->philos[index], index);
}
