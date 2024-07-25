/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 04:36:18 by dstinghe          #+#    #+#             */
/*   Updated: 2024/07/25 05:56:39 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void *philo_thread(void *arg)
{
	t_philo_t *philo;

	philo = (t_philo_t *)arg;
	set_time_start(philo);
	set_time_think(philo);
	philo_synch(philo);
	while (thread_check(philo) == false)
	{
		philo_iteration_latency(philo);
		if ((philo->status == THINK && philo->time_event >= philo->time_think) ||
		(philo->status == START && philo->time >= philo->time_start))
			set_status(philo, EAT);
		if (philo->status != EAT)
			reset_forks(philo);
		if (philo->status == EAT && philo->time_event >= philo->time_eat)
			set_status(philo, SLEEP);
		if (philo->eat_count == 0)
			set_thread(philo, ENDED);
		if (philo->status == SLEEP && philo->time_event >= philo->time_sleep)
			set_status(philo, THINK);
		if (philo->time_eat_last >= philo->time_death)
			set_status(philo, DEAD);
		usleep(50);
	}
	return (NULL);
}

void set_time_start(t_philo_t *philo)
{
	if (philo->philo_id % 2 == 0)
		philo->time_start = 0;
	else
		philo->time_start = philo->time_eat;
	if (philo->philo_total % 2 != 0 && philo->philo_id == philo->philo_total)
		philo->time_start = philo->time_eat * 2;
	if (philo->philo_total == 1)
		philo->time_start = philo->time_death + 100;
}

void set_time_think(t_philo_t *philo)
{
	if (philo->philo_total % 2 == 0)
		philo->time_think = philo->time_eat - philo->time_sleep;
	else
		philo->time_think = (philo->time_eat * 2) - philo->time_sleep;
}

void set_thread(t_philo_t *philo, t_thread thread)
{
	pthread_mutex_lock(philo->lock_thread);
	*philo->thread = thread;
	pthread_mutex_unlock(philo->lock_thread);
}

bool thread_check(t_philo_t *philo)
{
	bool ret_value;

	ret_value = false;
	pthread_mutex_lock(philo->lock_thread);
	if (*philo->thread == STOPPED)
		ret_value = true;
	pthread_mutex_unlock(philo->lock_thread);
	return (ret_value);
}

void set_status(t_philo_t *philo, t_status_t status)
{
	if (status == EAT && philo->time_eat_last < philo->time_death &&
	philo->eat_count != 0)
	{
		get_forks(philo);
		put_stdout(philo, "is eating", 0);
		philo->time_eat_last = 0;
	}
	else if (status == SLEEP && philo->time_eat_last < philo->time_death &&
	philo->time_sleep > 0)
		put_stdout(philo, "is sleeping", 0);
	else if (status == THINK && philo->time_eat_last < philo->time_death &&
	philo->time_think > 0)
		put_stdout(philo, "is thinking", 0);
	else if (status == DEAD)
	{
		put_stdout(philo, "died", 0);
		set_thread(philo, STOPPED);
	}
	philo->status = status;
	philo->time_event = 0;
}

void philo_iteration_latency(t_philo_t *philo)
{
	uint64_t time_current;
	
	if (init_time(&time_current))
	{
		put_stdout(philo, ERROR_GETTIMEOFDAY, 1);
		set_thread(philo, STOPPED);
		return ;
	}
	if (time_current >= philo->time_synch_start)
	{
		philo->time_synch_start += 1000;
		philo->time_event++;
		philo->time_eat_last++;
		philo->time++;
	}
}

void get_forks(t_philo_t *philo)
{
	while (philo->fork_count < 2)
	{
		pthread_mutex_lock(philo->lock_fork_left);
		if (*philo->fork_left == FORK)
		{
			*philo->fork_left = NO_FORK;
			philo->fork_count += 1;
			put_stdout(philo, "has taken a fork", 0);
		}
		pthread_mutex_unlock(philo->lock_fork_left);
		pthread_mutex_lock(philo->lock_fork_right);
		if (*philo->fork_right == FORK)
		{
			*philo->fork_right = NO_FORK;
			philo->fork_count += 1;
			put_stdout(philo, "has taken a fork", 0);
		}
		pthread_mutex_unlock(philo->lock_fork_right);
	}
}

void reset_forks(t_philo_t *philo)
{
	if (philo->fork_count == 2)
	{
		pthread_mutex_lock(philo->lock_fork_left);
		*philo->fork_left = FORK;
		pthread_mutex_unlock(philo->lock_fork_left);
		pthread_mutex_lock(philo->lock_fork_right);
		*philo->fork_right = FORK;
		pthread_mutex_unlock(philo->lock_fork_right);
		philo->fork_count = 0;
		philo->eat_count--;
	}
}

void philo_synch(t_philo_t *philo)
{
	uint64_t time_current;

	if (init_time(&time_current))
	{
		put_stdout(philo, ERROR_GETTIMEOFDAY, 1);
		set_thread(philo, STOPPED);
		return ;
	}
	while (time_current < philo->time_synch_start)
	{
		if (init_time(&time_current))
		{
			put_stdout(philo, ERROR_GETTIMEOFDAY, 1);
			set_thread(philo, STOPPED);
			return ;
		}
		usleep(100);
	}
	philo->time_synch_start += 1000;
}

void put_stdout(t_philo_t *philo, char *action, int error)
{
	pthread_mutex_lock(philo->lock_printf);
	if (error)
		printf("%s", ERROR_GETTIMEOFDAY);
	else if (*philo->someone_died == false)
		printf("%d %d %s\n", philo->time, philo->philo_id, action);
	if (!ft_strncmp(action, "died", 4))
		*philo->someone_died = true;
	pthread_mutex_unlock(philo->lock_printf);
}
