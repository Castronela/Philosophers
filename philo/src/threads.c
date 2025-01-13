/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 19:16:18 by david             #+#    #+#             */
/*   Updated: 2025/01/13 15:56:31 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			threads_main(t_philo_data *data);

static int	threads_launch(t_philo_data *data);
static void	threads_join(t_philo_data *data, int index);

int	threads_main(t_philo_data *data)
{
	if (threads_launch(data))
	{
		data_free(data, 1);
		return (EXIT_FAILURE);
	}
	if (threads_monitor(data))
	{
		threads_join(data, data->philo_count);
		data_free(data, 1);
		return (EXIT_FAILURE);
	}
	threads_join(data, data->philo_count);
	return (EXIT_SUCCESS);
}

static int	threads_launch(t_philo_data *data)
{
	size_t	index;

	index = 0;
	while (index < data->philo_count)
	{
		if (pthread_create(&data->philos[index].thread, NULL, philo_thread,
				(void *)&data->philos[index]))
		{
			printf("%s\n", ERRMSG_INTERNAL ERRMSG_THREAD_CR);
			threads_stop(data);
			threads_join(data, index);
			return (EXIT_FAILURE);
		}
		index++;
	}
	return (EXIT_SUCCESS);
}

static void	threads_join(t_philo_data *data, int index)
{
	while (--index >= 0)
	{
		if (pthread_join(data->philos[index].thread, NULL))
			printf("%s\n", ERRMSG_INTERNAL ERRMSG_THREAD_JO);
	}
}
