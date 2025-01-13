/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:28:56 by dstinghe          #+#    #+#             */
/*   Updated: 2025/01/13 15:41:42 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	forks_pickup(t_philos *philo);
int	forks_putback(t_philos *philo);

int	forks_pickup(t_philos *philo)
{
	if (pthread_mutex_lock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (*philo->fork_right)
	{
		*philo->fork_right = 0;
		philo->fork_count++;
	}
	if (pthread_mutex_unlock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	if (pthread_mutex_lock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	if (*philo->fork_left)
	{
		*philo->fork_left = 0;
		philo->fork_count++;
	}
	if (pthread_mutex_unlock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	return (EXIT_SUCCESS);
}

int	forks_putback(t_philos *philo)
{
	if (pthread_mutex_lock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	*philo->fork_left = 1;
	philo->fork_count--;
	if (pthread_mutex_unlock(philo->lock_fork_left))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	if (pthread_mutex_lock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_LOCK), 1);
	*philo->fork_right = 1;
	philo->fork_count--;
	if (pthread_mutex_unlock(philo->lock_fork_right))
		return (printf("%s\n", ERRMSG_INTERNAL ERRMSG_MUTEX_UNLOCK), 1);
	return (EXIT_SUCCESS);
}
