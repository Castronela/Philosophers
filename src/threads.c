/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 04:22:03 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/25 06:44:25 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thread_join(pthread_t *thread, int i)
{
	while (--i >= 0)
		pthread_join(thread[i], NULL);
}

int	thread_handler(t_data_t *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_total)
	{
		if (pthread_create(&data->philo_thread[i], NULL, philo_thread,
				(void *)&data->philo[i]))
		{
			if (i > 0)
			{
				term_all(data);
				thread_join(data->philo_thread, i);
			}
			error(data, ERROR_THREAD_LAUNCH, 2);
			return (1);
		}
	}
	term_thread(data);
	thread_join(data->philo_thread, data->philo_total);
	mutex_destroy(data->lock_forks, data->philo_total);
	mutex_destroy(data->lock_thread, data->philo_total);
	pthread_mutex_destroy(&data->lock_printf);
	clean(data);
	return (0);
}

void	term_thread(t_data_t *data)
{
	while (term_check(data) == false)
		usleep(100);
	term_all(data);
}

bool	term_check(t_data_t *data)
{
	int		i;
	int		threads_ended;
	bool	ret_value;

	ret_value = false;
	threads_ended = 0;
	i = -1;
	while (++i < data->philo_total)
	{
		pthread_mutex_lock(&data->lock_thread[i]);
		if (data->thread_status[i] == STOPPED)
			ret_value = true;
		else if (data->thread_status[i] == ENDED)
			threads_ended++;
		pthread_mutex_unlock(&data->lock_thread[i]);
		if (ret_value == true)
			break ;
	}
	if (threads_ended == data->philo_total)
		ret_value = true;
	return (ret_value);
}

void	term_all(t_data_t *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_total)
	{
		pthread_mutex_lock(&data->lock_thread[i]);
		data->thread_status[i] = STOPPED;
		pthread_mutex_unlock(&data->lock_thread[i]);
	}
}
