/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:04:41 by david             #+#    #+#             */
/*   Updated: 2025/01/12 16:29:16 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int64_t	get_timeofday_ms(void);
int	update_time(t_philos *philo, uint64_t current_time_ms);
static void	print_action(t_philos *philo, const int action);
int	print_safe(t_philos *philo, const int action);
void	data_free(t_philo_data *data, const int destroy_mutex);

int64_t	get_timeofday_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
        printf("%s\n", ERRMSG_INTERNAL ERRMSG_TIMEOFDAY);
		return (0);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	update_time(t_philos *philo, uint64_t current_time_ms)
{
	if (!current_time_ms)
		current_time_ms = get_timeofday_ms();
	if (!current_time_ms)
		return (EXIT_FAILURE);
	if (current_time_ms > philo->time_start_ms)
	{
		philo->time_ms += current_time_ms - philo->time_start_ms;
		philo->time_start_ms = current_time_ms;
	}
	return (EXIT_SUCCESS);
}

/*
1 - thinking
2 - eating
3 - sleeping
4 - dead
*/
static void	print_action(t_philos *philo, const int action)
{
	if (action == 1)
		printf("%d %d is thinking\n", philo->time_ms, philo->philo_id);
	else if (action == 2)
	{
		printf("%d %d has taken a fork\n%d %d has taken a fork\n%d %d is eating\n", philo->time_ms, philo->philo_id, philo->time_ms, 
		philo->philo_id, philo->time_ms, philo->philo_id);
	}
	else if (action == 3)
		printf("%d %d is sleeping\n", philo->time_ms, philo->philo_id);
	if (action == 4)
		printf("%d %d died\n", philo->time_ms, philo->philo_id);
}

int	print_safe(t_philos *philo, const int action)
{
	// if (!(philo->philo_id & 1))
	// {
	// 	update_time(philo, 0);
	// 	printf("%d %d EATING\n", philo->time_ms, philo->philo_id);
	// }
	if (pthread_mutex_lock(philo->lock_printf))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK),
			EXIT_FAILURE);
	if (!(*philo->stop_printf))
	{
        if (update_time(philo, 0))
            return (EXIT_FAILURE);
		print_action(philo, action);
        if (philo->status != RUNNING && philo->status != EXITED)
            *philo->stop_printf = true;
	}
	if(pthread_mutex_unlock(philo->lock_printf))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK),
			EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	data_free(t_philo_data *data, const int destroy_mutex)
{
	if (destroy_mutex)
	{
		mutex_destroy_arr(&data->lock_fork, data->philo_count);
		mutex_destroy_arr(&data->lock_threads, data->philo_count);
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
