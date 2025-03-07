/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:31:01 by david             #+#    #+#             */
/*   Updated: 2025/01/31 03:59:59 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_philo_data	data;

	data = (t_philo_data){.eat_count = -1};
	if (is_input_valid(&data, ac, av) == false)
		return (EXIT_FAILURE);
	if (data_init(&data))
		return (EXIT_FAILURE);
	if (threads_main(&data))
		return (EXIT_FAILURE);
	data_free(&data, 1);
	return (EXIT_SUCCESS);
}
