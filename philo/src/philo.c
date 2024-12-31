/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:31:01 by david             #+#    #+#             */
/*   Updated: 2024/12/31 15:38:45 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int main(int ac, char **av)
{
    t_philo_data data;
    
    data.eat_count = -1;
    if (is_input_valid(&data, ac, av) == false)
        return (EXIT_FAILURE);
    if (data_alloc(&data))
        return (EXIT_FAILURE);
    data_init(&data);
    // test_print_data(&data, 30);
    if (threads_main(&data))
        return (EXIT_FAILURE);
    data_free(&data, 1);
}

