/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 19:32:59 by david             #+#    #+#             */
/*   Updated: 2024/12/31 16:20:26 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void *philo_thread(void *philo_void);

int philo_cycle(t_philos *philo);
void check_eating_status(t_philos *philo);
void check_stopped_thread(t_philos *philo);
void print_status(t_philos *philo, const t_state new_state);
void change_status(t_philos *philo, t_state new_state);
void forks_pickup(t_philos *philo);
void forks_putback(t_philos *philo);
int update_time(t_philos *philo);
int apply_start_delay(t_philos *philo);
void philo_init(t_philos *philo);

void set_time_think(t_philos *philo);

void *philo_thread(void *philo_void)
{
    t_philos *philo;

    philo = philo_void;
    philo_init(philo);
    // printf("%d thinks: %d\n", philo->philo_id, philo->time_last_activity);
    apply_start_delay(philo);
    // printf("%d starts at %d (%llu)\n", philo->philo_id, philo->time_ms, philo->time_start_ms);
    philo_cycle(philo);
    return (NULL);
}

int philo_cycle(t_philos *philo)
{
    while (philo->status == THINKING || philo->status == EATING || philo->status == SLEEPING)
    {
        if (philo->status == THINKING && !philo->time_last_activity)
            change_status(philo, EATING);
        else if (philo->status == EATING && !philo->time_last_activity)
            change_status(philo, SLEEPING);
        else if (philo->status == SLEEPING && !philo->time_last_activity)
            change_status(philo, THINKING);
        if (update_time(philo))
            return (EXIT_FAILURE);
        check_eating_status(philo);
        check_stopped_thread(philo);
    }
    return (EXIT_SUCCESS);
}

void check_eating_status(t_philos *philo)
{
    if (philo->time_die <= philo->time_last_eat)
        change_status(philo, DEAD);
    else if (!philo->eat_count)
        philo->status = EXITED;     
}

void check_stopped_thread(t_philos *philo)
{
    pthread_mutex_lock(philo->lock_printf);
    if (*philo->stop_threads)
        philo->status = STOPPED;
    pthread_mutex_unlock(philo->lock_printf);
}

void print_status(t_philos *philo, const t_state new_state)
{
    if (new_state == philo->status)
        return ;
    philo->status = new_state;
    if (new_state == THINKING && philo->time_think)
    {
        philo->time_last_activity = philo->time_think;
        print_safe(philo, 1, "is thinking");
    }
    else if (new_state == EATING)
    {
        set_time_think(philo);
        philo->time_last_activity = philo->time_eat;
        print_safe(philo, 1, "is eating");
    }
    else if (new_state == SLEEPING)
    {
        philo->time_last_activity = philo->time_sleep;
        print_safe(philo, 1, "is sleeping");
    }
    else if (new_state == DEAD)
        print_safe(philo, 1, "died");
}

void change_status(t_philos *philo, t_state new_state)
{
    if (new_state == EATING)
    {
        philo->test_eating_counter--;
        philo->time_last_eat = 0;
    }
    else if (new_state == SLEEPING)
        philo->eat_count--;
    print_status(philo, new_state);
}

void set_time_think(t_philos *philo)
{
    if (!(philo->philo_count & 1) || philo->time_sleep > philo->time_eat)
        return ;
    if (!philo->test_eating_counter)
    {
        philo->time_think = (philo->time_eat * 2) - philo->time_sleep;
    }
    else if (philo->test_eating_counter < 0)
    {
        philo->time_think = philo->time_eat - philo->time_sleep;
        philo->test_eating_counter = philo->philo_count / 2 - 1;
    }
}

void forks_pickup(t_philos *philo)
{
    pthread_mutex_lock(philo->lock_fork_left);
    if (*philo->fork_left)
    {
        *philo->fork_left = 0;
        philo->fork_count++;
    }
    pthread_mutex_unlock(philo->lock_fork_left);
    pthread_mutex_lock(philo->lock_fork_right);
    if (*philo->fork_right && philo->fork_count)
    {
        *philo->fork_right = 0;
        philo->fork_count++;
    }
    pthread_mutex_unlock(philo->lock_fork_right);
    if (philo->fork_count == 1)
    {
        philo->fork_count = 0;
        pthread_mutex_lock(philo->lock_fork_left);
        *philo->fork_left = 1;
        pthread_mutex_unlock(philo->lock_fork_left);
    }
    else if (philo->fork_count == 2)
    {
        print_safe(philo, 1, "has taken a fork");
        print_safe(philo, 1, "has taken a fork");
    }
}

void forks_putback(t_philos *philo)
{
    pthread_mutex_lock(philo->lock_fork_left);
    *philo->fork_left = 1;
    pthread_mutex_unlock(philo->lock_fork_left);
    pthread_mutex_lock(philo->lock_fork_right);
    *philo->fork_right = 1;
    pthread_mutex_unlock(philo->lock_fork_right);
    // print_safe(philo, 1, "has put back forks");
    philo->fork_count = 0;
}

int update_time(t_philos *philo)
{
    uint64_t current_time_ms;

    current_time_ms = get_timeofday_ms(1, philo);
    if (!current_time_ms)
    {
        philo->status = STOPPED;
        return (EXIT_FAILURE);
    }
    if (current_time_ms > philo->time_start_ms)
    {
        philo->time_ms++;
        philo->time_last_activity--;
        philo->time_last_eat++;
        philo->time_start_ms = current_time_ms;
    }
    return (EXIT_SUCCESS);
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
    if (!(philo->philo_id & 1))
    {
        philo->time_last_activity = philo->time_eat;
    }
    else if (philo->philo_id == philo->philo_count && philo->philo_count & 1)
    {
        philo->time_last_activity = philo->time_eat * 2;
    }
    else
        philo->time_last_activity = 0;
    
    if (philo->philo_id == 1)
        philo->test_eating_counter = 1;
    else if (!(philo->philo_id & 1) || philo->philo_id == philo->philo_count)
        philo->test_eating_counter = philo->philo_id / 2;
    else
        philo->test_eating_counter = philo->philo_id / 2 + 1;
        
        
    philo->time_ms = 0;
    philo->time_think = 0;
    if (philo->time_sleep <= philo->time_eat)
        philo->time_think = philo->time_eat - philo->time_sleep;
    philo->status = THINKING;
    philo->fork_count = 0;
    philo->time_last_eat = 0;
}

/*

9 600 200 200 g
9 600 150 400 g
9 600 400 150 g

8 600 200 200 g
8 600 150 400 g
8 600 400 150

*/