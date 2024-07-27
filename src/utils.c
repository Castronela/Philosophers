/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:50:38 by castronela        #+#    #+#             */
/*   Updated: 2024/07/27 07:27:25 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_whitespace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (true);
	else
		return (false);
}

bool	is_digit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

bool	input_valid(char *av)
{
	int	num_count;
	int	digit_count;

	if (av == NULL)
		return (false);
	num_count = 0;
	digit_count = 0;
	if (is_digit(*av))
		num_count++;
	while (*av)
	{
		if (is_digit(*av))
			digit_count++;
		if (digit_count > INT_MAX_DIGIT_COUNT)
			return (false);
		if (is_digit(*av) == false && is_whitespace(*av) == false)
			return (false);
		if (is_whitespace(*av) && *(av + 1) && is_digit(*(av + 1)))
			num_count++;
		if (num_count > 1 && is_digit(*av))
			return (false);
		av++;
	}
	return (true);
}

int	error(t_data_t *data, char *err_msg, int clean_all)
{
	if (clean_all > 1)
	{
		mutex_destroy(data->lock_forks, data->philo_total);
		mutex_destroy(data->lock_thread, data->philo_total);
		pthread_mutex_destroy(&data->lock_printf);
		clean(data);
	}
	printf("%s", err_msg);
	return (1);
}

int	init_time(uint64_t *time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (1);
	*time = (uint64_t)(tv.tv_sec * 1000000) + (uint64_t)tv.tv_usec;
	return (0);
}
