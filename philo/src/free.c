/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:04:41 by david             #+#    #+#             */
/*   Updated: 2025/01/13 16:00:53 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void		data_free(t_philo_data *data, const int destroy_mutex);

void	data_free(t_philo_data *data, const int destroy_mutex)
{
	if (destroy_mutex)
	{
		mutex_destroy_arr(data->lock_fork, data->philo_count);
		mutex_destroy_arr(data->lock_threads, data->philo_count);
		if (pthread_mutex_destroy(&data->lock_printf))
			printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
	}
	if (data->table_w_forks)
		free(data->table_w_forks);
	if (data->lock_fork)
		free(data->lock_fork);
	if (data->philos)
		free(data->philos);
	if (data->stop_threads)
		free(data->stop_threads);
	if (data->lock_threads)
		free(data->lock_threads);
}
