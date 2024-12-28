/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:31:01 by david             #+#    #+#             */
/*   Updated: 2024/12/28 21:03:33 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

int main(int ac, char **av)
{
    t_philo_data data;
    
    if (is_input_valid(&data, ac, av) == false)
        return (EXIT_FAILURE);
    else
        printf("Good!\n");
}

