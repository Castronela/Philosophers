/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:31:01 by david             #+#    #+#             */
/*   Updated: 2025/01/07 18:53:30 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_philo_data	data;

	data.eat_count = -1;
	if (is_input_valid(&data, ac, av) == false)
		return (EXIT_FAILURE);
	if (data_alloc(&data))
		return (EXIT_FAILURE);
	data_init(&data);
	if (threads_main(&data))
		return (EXIT_FAILURE);
	data_free(&data, 1);
}
