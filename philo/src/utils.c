/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 16:04:41 by david             #+#    #+#             */
/*   Updated: 2024/12/29 18:58:48 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int64_t get_timeofday_ms(pthread_mutex_t *lock_printf);
int print_safe(const char *err_msg, int exit_code, pthread_mutex_t *lock_printf);

int64_t get_timeofday_ms(pthread_mutex_t *lock_printf)
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL) < 0)
        return (print_safe(ERRMSG_INTERNAL ERRMSG_TIMEOFDAY, EXIT_FAILURE, lock_printf));
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int print_safe(const char *err_msg, int exit_code, pthread_mutex_t *lock_printf)
{
    if (lock_printf)
        pthread_mutex_lock(lock_printf);
    printf("%s\n", err_msg);
    if (lock_printf)
        pthread_mutex_unlock(lock_printf);
    return (exit_code);
}
