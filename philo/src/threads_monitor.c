/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_monitor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:52:23 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/13 15:54:43 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int			threads_monitor(t_philo_data *data);
void		threads_stop(t_philo_data *data);

static int	threads_check(t_philo_data *data);

int	threads_monitor(t_philo_data *data)
{
	int	ret_code;

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

void	threads_stop(t_philo_data *data)
{
	size_t	index;

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
static int	threads_check(t_philo_data *data)
{
	size_t	index;
	int		ret_code;

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
