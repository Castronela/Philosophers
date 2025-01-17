/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:25:44 by castronela        #+#    #+#             */
/*   Updated: 2024/07/25 06:45:19 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	int			i;
	t_data_t	data;

	if (ac == 5 || ac == 6)
	{
		i = 0;
		while (++i < ac)
		{
			if (input_valid(av[i]) == false || ft_atoi(av[i]) > INT_MAX)
				return (error(&data, ERROR_INPUT, i));
			if (i == 1)
			{
				data.philo_total = ft_atoi(av[i]);
				if (data.philo_total == 0)
					return (error(&data, ERROR_INPUT, i));
				if (setup_data(&data))
					return (1);
			}
			else
				init_input_data(&data, i, (int)ft_atoi(av[i]));
		}
		return (thread_handler(&data));
	}
	return (error(&data, ERROR_INPUT, 0));
}
