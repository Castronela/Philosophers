/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 20:32:12 by david             #+#    #+#             */
/*   Updated: 2024/12/29 20:19:56 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int data_alloc(t_philo_data *data);
void data_free(t_philo_data *data, int destroy_mutex);

static int alloc_mem(t_philo_data *data);
static int mutex_init_all(t_philo_data *data);
static void mutex_destroy_all(t_philo_data *data, int index);

int data_alloc(t_philo_data *data)
{   
    data->time_start_ms = get_timeofday_ms(0, NULL);
    if (!data->time_start_ms)
        return (EXIT_FAILURE);
    data->forks = NULL;
    data->lock_fork = NULL;
    data->philos = NULL;
    if (alloc_mem(data))
        return (EXIT_FAILURE);
    if (mutex_init_all(data))
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

void data_free(t_philo_data *data, int destroy_mutex)
{
    if (data->forks)
        free(data->forks);
    if (data->lock_fork)
    {
        if (destroy_mutex)
            mutex_destroy_all(data, data->philo_count);
        free(data->lock_fork);
    }
    if (data->philos)
        free(data->philos);
    if (data->threads)
        free(data->threads);
}

static int alloc_mem(t_philo_data *data)
{
    size_t index;

    data->forks = malloc(sizeof(*data->forks) * data->philo_count);
    data->lock_fork = malloc(sizeof(*data->lock_fork) * data->philo_count);
    data->philos = malloc(sizeof(*data->philos) * data->philo_count);
    data->threads = malloc(sizeof(*data->threads) * data->philo_count);
    if (!data->lock_fork || !data->forks || !data->philos || !data->threads)
    {
        data_free(data, 0);
        printf("%s %s\n" ,ERRMSG_INTERNAL, ERRMSG_MALLOC);
        return (EXIT_FAILURE);
    }
    index = -1;
    while (++index < data->philo_count)
        data->forks[index] = 1;
    return (EXIT_SUCCESS);
}

static int mutex_init_all(t_philo_data *data)
{
    size_t index;

    if (pthread_mutex_init(&data->lock_printf, NULL))
    {
        printf("%s %s\n", ERRMSG_INTERNAL, ERRMSG_MUTEX_INIT);
        return (EXIT_FAILURE);
    }
    index = -1;
    while (++index < data->philo_count)
    {
        if (pthread_mutex_init(&data->lock_fork[index], NULL))
        {
            mutex_destroy_all(data, index);
            data_free(data, 0);
            printf("%s %s\n" ,ERRMSG_INTERNAL, ERRMSG_MUTEX_INIT);
            return (EXIT_FAILURE);
        }
    }
    return (EXIT_SUCCESS);
}

static void mutex_destroy_all(t_philo_data *data, int index)
{
    while (--index >= 0)
    {
        if (pthread_mutex_destroy(&data->lock_fork[index]))
            printf("%s %s\n" ,ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
    }
    if (pthread_mutex_destroy(&data->lock_printf))
        printf("%s %s\n" ,ERRMSG_INTERNAL, ERRMSG_MUTEX_DESTR);
}
