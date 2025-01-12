/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 19:16:18 by david             #+#    #+#             */
/*   Updated: 2025/01/12 16:41:51 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	threads_main(t_philo_data *data);

static int threads_monitor(t_philo_data *data);
static int threads_check(t_philo_data *data);
static int	threads_launch(t_philo_data *data);
static void	threads_join(t_philo_data *data, int index);
static void threads_stop(t_philo_data *data);

int	threads_main(t_philo_data *data)
{
	if (threads_launch(data))
		return (EXIT_FAILURE);
	if (threads_monitor(data))
	{
		threads_join(data, data->philo_count);
		return (EXIT_FAILURE);
	}
	threads_join(data, data->philo_count);
	return (EXIT_SUCCESS);
}

static int threads_monitor(t_philo_data *data)
{
	int ret_code;

	ret_code = 0;
	while (!ret_code)
	{
		usleep(USLEEP_VALUE);
		ret_code = threads_check(data);
	}
	if (ret_code != 2)
		threads_stop(data);
	if (ret_code == 3)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static void threads_stop(t_philo_data *data)
{
	size_t index;

	index = 0;
	while (index < data->philo_count)
	{
		if (pthread_mutex_lock(&data->lock_threads[index]))
			printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK);
		if (data->stop_threads[index] != STOPPED)
			data->stop_threads[index] = STOPPED;
		if (pthread_mutex_unlock(&data->lock_threads[index]))
			printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK);
		index++;
	}
}

/*
0 - threads still running
1 - someone died: stop all threads, exit success
2 - someone exited: return from monitor, exit success
3 - internal failure: stop all threads, exit failure
*/
static int threads_check(t_philo_data *data)
{
	size_t index;
	int ret_code;

	ret_code = 0;
	index = 0;
	while (!ret_code && index < data->philo_count)
	{
		if (pthread_mutex_lock(&data->lock_threads[index]))
			printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK);
		if (data->stop_threads[index] == DEAD)
			ret_code = 1;
		else if (data->stop_threads[index] == EXITED)
			ret_code = 2;
		else if (data->stop_threads[index] == INT_FAIL)
			ret_code = 3;
		if (pthread_mutex_unlock(&data->lock_threads[index]))
			printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK);
		index++;
	}
	return (ret_code);
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
			data_free(data, 1);
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
