/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:50:38 by castronela        #+#    #+#             */
/*   Updated: 2024/07/25 05:45:52 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool is_whitespace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (true);
	else
		return (false);
}

bool is_digit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

bool input_valid(char *av)
{
	int num_count;
	int digit_count;
	
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
		if (is_whitespace(*av) && *(av+1) && is_digit(*(av+1)))
			num_count++;
		if (num_count > 1 && is_digit(*av))
			return (false);		
		av++;
	}
	return (true);
}

int error(t_data_t *data, char *err_msg, int clean_all)
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

long long ft_atoi(char *av)
{
    long long num;
    
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

int ft_strncmp(char *s1, char *s2, int n)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2 && *s1 == *s2 && n)
	{
		s1++;
		s2++;
		n--;
	}
	return (*s1 - *s2);
}

void init_input_data(t_data_t *data, int ac, int value)
{
	int i;

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
			data->philo[i].time_sleep = value;
		else if (ac == 5)
			data->philo[i].eat_count = value;		
	}
}

void init_data(t_data_t *data, int i)
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

int setup_data(t_data_t *data)
{
	if (memalloc_data(data))
		return (error(data, ERROR_MEM_ALLOC, 0));
	if (init_time(&data->time_synch_start))
		return (error(data, ERROR_GETTIMEOFDAY, 2));
	if (init_mutex(data))
		return (error(data, ERROR_MUTEX_INIT, 2));
	return (0);
}

int memalloc_data(t_data_t *data)
{
	data->philo = malloc(data->philo_total * sizeof(t_philo_t));
	if (data->philo == NULL)
		return (1);
	data->philo_thread = malloc(data->philo_total * sizeof(pthread_t));
	if (data->philo_thread == NULL)		
	{
		free (data->philo);
		return (1);
	}
	if (vars_allocmem(data))
	{
		free (data->philo_thread);
		free (data->philo);
		return (1);		
	}
	if (mutex_allocmem(data))
	{
		free (data->thread_status);
		free (data->forks);
		free (data->philo_thread);
		free (data->philo);
		return (1);		
	}
	return (0);
}

int init_time(uint64_t *time)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return (1);
	*time = (uint64_t) (tv.tv_sec * 1000000) + (uint64_t) tv.tv_usec;;
	return (0);
}

int init_mutex(t_data_t *data)
{
	int i;

	if (pthread_mutex_init(&data->lock_printf, NULL))
		return (1);
	i = -1;
	while (++i < data->philo_total)
	{
		if (pthread_mutex_init(&data->lock_forks[i], NULL))
		{
			pthread_mutex_destroy(&data->lock_printf);
			mutex_destroy(data->lock_forks, i);
			return (1);
		}
		if (pthread_mutex_init(&data->lock_thread[i], NULL))
		{
			pthread_mutex_destroy(&data->lock_forks[i]);
			mutex_destroy(data->lock_forks, i + 1);
			mutex_destroy(data->lock_thread, i);
			return (1);			
		}
	}
	return (0);
}

void mutex_destroy(pthread_mutex_t *lock, int i)
{
	while (--i >= 0)
		pthread_mutex_destroy(&lock[i]);
}

void clean(t_data_t *data)
{
	free(data->lock_thread);
	free(data->thread_status);
	free(data->lock_forks);
	free(data->forks);
	free(data->philo_thread);
	free(data->philo);
}

int vars_allocmem(t_data_t *data)
{
	int i;

	data->forks = malloc(data->philo_total * sizeof(t_fork));
	if (data->forks == NULL)
		return (1);
	data->thread_status = malloc(data->philo_total * sizeof(t_thread));
	if (data->thread_status == NULL)
	{
		free(data->forks);
		return (1);
	}
	i = -1;
	while (++i < data->philo_total)
	{
		data->philo[i].fork_left = &data->forks[i];
		if (i + 1 == data->philo_total)
			data->philo[i].fork_right = &data->forks[0];
		else
			data->philo[i].fork_right = &data->forks[i + 1];
		data->philo[i].thread = &data->thread_status[i];
		data->philo[i].lock_printf = &data->lock_printf;
		data->philo[i].someone_died = &data->someone_died;
	}
	return (0);
}

int mutex_allocmem(t_data_t *data)
{
	int i;
	data->lock_forks = malloc(data->philo_total * sizeof(pthread_mutex_t));
	if (data->lock_forks == NULL)
		return (1);
	data->lock_thread = malloc(data->philo_total * sizeof(pthread_mutex_t));
	if (data->lock_thread == NULL)
	{
		free(data->lock_forks);
		return (1);
	}
	i = -1;
	while (++i < data->philo_total)
	{
		data->philo[i].lock_fork_left = &data->lock_forks[i];
		if (i + 1 == data->philo_total)
			data->philo[i].lock_fork_right = &data->lock_forks[0];
		else
			data->philo[i].lock_fork_right = &data->lock_forks[i + 1];
		data->philo[i].lock_thread = &data->lock_thread[i];
		data->philo[i].lock_printf = &data->lock_printf;				
	}
	return (0);
}
