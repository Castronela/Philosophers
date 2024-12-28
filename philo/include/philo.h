/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:58 by david             #+#    #+#             */
/*   Updated: 2024/12/28 21:35:20 by david            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>

# define INT_MAX_DIGITS 10

# define COLOR_RED "\033[0;31m"
# define COLOR_RESET "\033[0m"

# define ERRMSG COLOR_RED"Error: "COLOR_RESET
# define ERRMSG_ARG_FEW "Too few arguments:"
# define ERRMSG_ARG_MANY "Too many arguments:"
# define ERRMSG_NUM_INVALID "Invalid number:"
# define ERRMSG_NUM_LARGE "Argument size larger than 'int' capacity:"
# define ERRMSG_NUM_NEG "Number must be positive:"

typedef enum e_invalid_input_type {
    VALID,
    ARGS_TOO_FEW,     
    ARGS_TOO_MANY,   
    INVALID,
    TOO_LARGE,
    NEGATIVE
}   t_invalid_input_type;

typedef struct s_philo_data {
    size_t philo_count;
    size_t time_die;
    size_t time_eat;
    size_t time_sleep;
    size_t eat_count;
}   t_philo_data;


int main(int ac, char **av);

// ---- Input validation -----------------------------------------------------------------

bool is_input_valid(t_philo_data *data, size_t ac, char **av);

# endif