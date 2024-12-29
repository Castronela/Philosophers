/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 19:16:18 by david             #+#    #+#             */
/*   Updated: 2024/12/29 20:17:06 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int threads_main(t_philo_data *data);

static int threads_launch(t_philo_data *data);
static void threads_join(t_philo_data *data, int index);

int threads_main(t_philo_data *data)
{
    if (threads_launch(data))
        return (EXIT_FAILURE);
    threads_join(data, data->philo_count);
    return (EXIT_SUCCESS);
}

static int threads_launch(t_philo_data *data)
{
    size_t index;
    
    index = 0;
    while (index < data->philo_count)
    {
        if (pthread_create(&data->threads[index], NULL, philo_thread, (void *)&data->philos[index]))
        {
            print_safe(&data->philos[index], 0, ERRMSG_INTERNAL ERRMSG_THREAD_CR);
            threads_join(data, index);
            data_free(data, 1);
            return (EXIT_FAILURE);
        }
        index++;
    }
    return (EXIT_SUCCESS);
}

static void threads_join(t_philo_data *data, int index)
{
    while (--index >= 0)
    {
        if (pthread_join(data->threads[index], NULL))
            print_safe(&data->philos[index], 0, ERRMSG_INTERNAL ERRMSG_THREAD_JO);
    }
}
