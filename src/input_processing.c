/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 06:32:25 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/25 06:46:05 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_input_data(t_data_t *data, int ac, int value)
{
	int	i;

	data->time_synch_start += SYNCH_DELAY_U_SEC;
	data->someone_died = false;
	i = -1;
	while (++i < data->philo_total)
	{
		if (ac == 2)
		{
			data->philo[i].time_death = value;
			init_data(data, i);
		}
		else if (ac == 3)
			data->philo[i].time_eat = value;
		else if (ac == 4)
		{
			data->philo[i].time_sleep = value;
			data->philo[i].time_start = get_time_start(data, i + 1);
			data->philo[i].time_think = get_time_think(data);
		}
		else if (ac == 5)
			data->philo[i].eat_count = value;
	}
}

void	init_data(t_data_t *data, int i)
{
	data->philo[i].time_synch_start = data->time_synch_start;
	data->philo[i].philo_total = data->philo_total;
	data->philo[i].philo_id = i + 1;
	data->philo[i].fork_count = 0;
	data->philo[i].status = START;
	data->philo[i].time = 0;
	data->philo[i].time_event = 0;
	data->philo[i].time_eat_last = 0;
	data->philo[i].eat_count = -1;
	data->forks[i] = FORK;
	data->thread_status[i] = RUNNING;
}

int	get_time_start(t_data_t *data, int philo_id)
{
	int	ret_time;

	if (philo_id % 2 == 0)
		ret_time = 0;
	else
		ret_time = data->philo->time_eat;
	if (data->philo_total % 2 != 0 && philo_id == data->philo_total)
		ret_time = data->philo->time_eat * 2;
	if (data->philo->philo_total == 1)
		ret_time = data->philo->time_death + 100;
	return (ret_time);
}

int	get_time_think(t_data_t *data)
{
	int	ret_time;

	if (data->philo->philo_total % 2 == 0)
		ret_time = data->philo->time_eat - data->philo->time_sleep;
	else
		ret_time = (data->philo->time_eat * 2) - data->philo->time_sleep;
	return (ret_time);
}

long long	ft_atoi(char *av)
{
	long long	num;

	while (is_whitespace(*av))
		av++;
	num = 0;
	while (is_digit(*av))
	{
		num *= 10;
		num += (long long)(*av - '0');
		av++;
	}
	return (num);
}
