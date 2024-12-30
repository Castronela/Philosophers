/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:04:41 by david             #+#    #+#             */
/*   Updated: 2024/12/30 18:59:04 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int64_t get_timeofday_ms(const bool safe, t_philos *philo);
void print_safe(t_philos *philo, const bool is_philo_state, const char *msg);

int64_t get_timeofday_ms(const bool safe, t_philos *philo)
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL) < 0)
    {
        if (safe)
            print_safe(philo, 0, ERRMSG_INTERNAL ERRMSG_TIMEOFDAY);
        else
            printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_TIMEOFDAY);
        return (0);
    }
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void print_safe(t_philos *philo, const bool is_philo_state, const char *msg)
{
    pthread_mutex_lock(philo->lock_printf);
    if (is_philo_state)
    {
        if (!(*philo->stop_threads))
            printf("%d %d %s\n", philo->time_ms, philo->philo_id, msg);
    }
    else
        printf("%s\n", msg);
    if (philo->status == DEAD || philo->status == INT_FAIL)
        *philo->stop_threads = true;
    else if (*philo->stop_threads)
        philo->status = STOPPED;
    pthread_mutex_unlock(philo->lock_printf);
}
