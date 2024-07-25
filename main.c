/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:25:44 by castronela        #+#    #+#             */
/*   Updated: 2024/07/04 14:41:14 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	int i;
	long long num;
	data_t data;

	if (ac == 5 || ac == 6)
	{
		i = 0;
		data.eat_counter = -1;
		while (++i < ac)
		{
			if (input_valid(av[i]) == false)
				return (input_error());
			num = ft_atoi(av[i]);
			if (num > INT_MAX)
				return (input_error());
			init_data(&data, i, (int)num);
		}
		return (launch_threads(&data));		
	}
	return (1);
}

// printf("p_count: %d\ntime_death: %d\ntime_eat: %d\ntime_sleep: %d\neat_count: %d\n", data.philo_count, data.time_death, data.time_eat, data.time_sleep, data.eat_counter);
