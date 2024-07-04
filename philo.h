/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: castronela <castronela@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:11:22 by castronela        #+#    #+#             */
/*   Updated: 2024/07/04 14:39:41 by castronela       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

# define INPUT_ERROR "invalid input\n"
# define INT_MAX_DIGIT_COUNT 10

typedef struct data_s {
    int philo_count;
	int time_death;
	int time_eat;
	int time_sleep;
	int eat_counter;

	pthread_mutex_t lock;
	
	long double time_start;
	long double time_current;
}   data_t;

int	main(int ac, char **av);

/** Utils **/
bool input_valid(char *av);
bool is_digit(int c);
bool is_whitespace(int c);
int input_error(void);
long long ft_atoi(char *av);
void init_data(data_t *input, int ac, int value);

/** Threads **/
int launch_threads(data_t *data);

#endif