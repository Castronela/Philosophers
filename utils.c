/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 13:50:38 by castronela        #+#    #+#             */
/*   Updated: 2024/07/04 14:16:24 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool is_whitespace(int c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (true);
	else
		return (false);
}

bool is_digit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	else
		return (false);
}

bool input_valid(char *av)
{
	int num_count;
	int digit_count;
	
	if (av == NULL)
		return (false);
	num_count = 0;
	digit_count = 0;
	if (is_digit(*av))
		num_count++;
	while (*av)
	{
		if (is_digit(*av))
			digit_count++;
		if (digit_count > INT_MAX_DIGIT_COUNT)
			return (false);
		if (is_digit(*av) == false && is_whitespace(*av) == false)
			return (false);
		if (is_whitespace(*av) && *(av+1) && is_digit(*(av+1)))
			num_count++;
		if (num_count > 1 && is_digit(*av))
			return (false);		
		av++;
	}
	return (true);
}

int input_error(void)
{
	printf(INPUT_ERROR);
	return (1);
}

long long ft_atoi(char *av)
{
    long long num;
    
    while (is_whitespace(*av))
        av++;
    num = 0;
    while (is_digit(*av))
    {
        num *= 10;
        num += (long long)(*av - '0');
        av++;
    }
    return (num);    
}

void init_data(data_t *data, int ac, int value)
{
    if (ac == 1)
        data->philo_count = value;
    else if (ac == 2)
        data->time_death = value;
    else if (ac == 3)
        data->time_eat = value;
    else if (ac == 4)
        data->time_sleep = value;
    else if (ac == 5)
        data->eat_counter = value;
}
