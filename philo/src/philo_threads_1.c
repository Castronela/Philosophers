/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 19:32:59 by david             #+#    #+#             */
/*   Updated: 2025/01/05 15:56:02 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void *philo_thread(void *philo_void);
int philo_cycle(t_philos *philo);
void check_eating_status(t_philos *philo);
void check_stopped_thread(t_philos *philo);
void lock_forks(t_philos *philo);
void unlock_forks(t_philos *philo);
void apply_sleep(t_philos *philo, const unsigned int time_ms);
int apply_start_delay(t_philos *philo);
void philo_init(t_philos *philo);

void forks_pickup(t_philos *philo);
void forks_putback(t_philos *philo);

/*
TODO while thread is waiting for mutex to be unlocked, it can't check for philosopher death;
Threads should not keep mutex locked for entire meal, instead pick forks from table and unlock mutex;
*/

void *philo_thread(void *philo_void)
{
    t_philos *philo;

    philo = philo_void;
    philo_init(philo);
    apply_start_delay(philo);
    philo_cycle(philo);
    return (NULL);
}

int philo_cycle(t_philos *philo)
{
    while (philo->status == RUNNING)
    {
        lock_forks(philo);
        if (philo->status != RUNNING)
            break ;
        unlock_forks(philo);
    }
    return (EXIT_SUCCESS);
}

void check_eating_status(t_philos *philo)
{
    update_time(philo);
    if (philo->time_die <= philo->time_ms - philo->time_last_eat)
    {
        philo->status = DEAD;
        print_safe(philo, 1, "died");
    }
    else if (!philo->eat_count)
        philo->status = EXITED;   
}

void check_stopped_thread(t_philos *philo)
{
    pthread_mutex_lock(philo->lock_printf);
    if (*philo->stop_threads && philo->status == RUNNING)
        philo->status = STOPPED;
    pthread_mutex_unlock(philo->lock_printf);
}

void forks_pickup(t_philos *philo)
{
    pthread_mutex_lock(philo->lock_fork_right);
    if (*philo->fork_right)
    {
        *philo->fork_right = 0;
        philo->fork_count++;
    }
    pthread_mutex_unlock(philo->lock_fork_right);
    pthread_mutex_lock(philo->lock_fork_left);
    if (*philo->fork_left)
    {
        *philo->fork_left = 0;
        philo->fork_count++;
    }
    pthread_mutex_unlock(philo->lock_fork_left);
}

void forks_putback(t_philos *philo)
{
    pthread_mutex_lock(philo->lock_fork_left);
    if (!(*philo->fork_left))
    {
        *philo->fork_left = 1;
        philo->fork_count--;
    }
    pthread_mutex_unlock(philo->lock_fork_left);
    pthread_mutex_lock(philo->lock_fork_right);
    if (!(*philo->fork_right))
    {
        *philo->fork_right = 1;
        philo->fork_count--;
    }
    pthread_mutex_unlock(philo->lock_fork_right);
}

void lock_forks(t_philos *philo)
{
    forks_pickup(philo);
    if (philo->fork_count < 2)
    {
        update_time(philo);
        print_safe(philo, 1, "is thinking");
    }
    while (philo->fork_count < 2)
    {
        check_eating_status(philo);
        check_stopped_thread(philo);
        if (philo->status != RUNNING)
            return ;
        forks_pickup(philo);
    }
    update_time(philo);
    print_safe(philo, 1, "has taken a fork");
    print_safe(philo, 1, "has taken a fork");
    print_safe(philo, 1, "is eating");
    philo->time_last_eat = philo->time_ms;
    apply_sleep(philo, philo->time_eat);
}

void unlock_forks(t_philos *philo)
{
    forks_putback(philo);
    print_safe(philo, 1, "is sleeping");
    philo->eat_count--;
    apply_sleep(philo, philo->time_sleep);
    // printf("%d time since last eat: %d\n", philo->philo_id, philo->time_ms - philo->time_last_eat);
}

void apply_sleep(t_philos *philo, const unsigned int time_ms)
{
    int64_t end_time;
    int64_t current_time;

    end_time = get_timeofday_ms(1, philo);
    if (!end_time)
    {
        philo->status = INT_FAIL;
        return ;
    }
    current_time = end_time;
    end_time += time_ms;
    while (current_time < end_time)
    {
        current_time = get_timeofday_ms(1, philo);
        if (!current_time)
            philo->status = INT_FAIL;
        check_eating_status(philo);
        check_stopped_thread(philo);
        if (philo->status != RUNNING)
            break ;
        usleep(100);
    }
}

int apply_start_delay(t_philos *philo)
{
    uint64_t current_time_ms;

    current_time_ms = get_timeofday_ms(1, philo);
    if (!current_time_ms)
    {
        philo->status = INT_FAIL;
        return (EXIT_FAILURE);
    }
    while (current_time_ms < philo->time_start_ms)
    {
        usleep(50);
        current_time_ms = get_timeofday_ms(1, philo);
        if (!current_time_ms)
        {
            philo->status = INT_FAIL;
            return (EXIT_FAILURE);
        }      
    }
    return (EXIT_SUCCESS);
}

void philo_init(t_philos *philo)
{          
    philo->time_ms = 0;
    philo->status = RUNNING;
    philo->time_last_eat = 0;
    philo->fork_count = 0;
    if (!(philo->philo_id & 1))
        forks_pickup(philo);     
}

/*

9 600 200 200 g
9 600 150 400 g
9 600 400 150 g

8 600 200 200 g
8 600 150 400 g
8 600 400 150

*/