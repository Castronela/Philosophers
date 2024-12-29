/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:58 by david             #+#    #+#             */
/*   Updated: 2024/12/29 19:23:08 by david            ###   ########.fr       */
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
# define START_DELAY_MS 1000

# define COLOR_RED "\033[0;31m"
# define COLOR_RESET "\033[0m"

# define ERRMSG_INPUT COLOR_RED"Input Error: "COLOR_RESET
# define ERRMSG_ARG_FEW "Too few arguments:"
# define ERRMSG_ARG_MANY "Too many arguments:"
# define ERRMSG_NUM_INVALID "Invalid number:"
# define ERRMSG_NUM_LARGE "Argument size larger than 'int' capacity:"
# define ERRMSG_NUM_NEG "Number must be positive:"

# define ERRMSG_INTERNAL COLOR_RED"Internal Error: "COLOR_RESET
# define ERRMSG_MALLOC "malloc"
# define ERRMSG_MUTEX_INIT "pthread_mutex_init"
# define ERRMSG_MUTEX_DESTR "pthread_mutex_destroy"
# define ERRMSG_TIMEOFDAY "timeofday"

typedef enum e_invalid_input_type {
    VALID,
    ARGS_TOO_FEW,     
    ARGS_TOO_MANY,   
    INVALID,
    TOO_LARGE,
    NEGATIVE
}   t_invalid_input_type;

typedef struct s_philos {
    uint64_t time_start_ms;
    unsigned int philo_id;
    unsigned int philo_count;
    
    unsigned int time_die;
    unsigned int time_eat;
    unsigned int time_sleep;
    int eat_count;

    unsigned int *fork_left;
    pthread_mutex_t *lock_fork_left;
    unsigned int *fork_right;
    pthread_mutex_t *lock_fork_right;
    
    pthread_mutex_t *lock_printf;
}   t_philos;

typedef struct s_philo_data {
    unsigned int time_start_ms;
    
    unsigned int philo_count;
    unsigned int time_die;
    unsigned int time_eat;
    unsigned int time_sleep;
    int eat_count;

    t_philos *philos;
    pthread_t *threads;
    
    unsigned int *forks;
    pthread_mutex_t *lock_fork;
    pthread_mutex_t lock_printf;
    
}   t_philo_data;


int main(int ac, char **av);

// ---- Input validation -----------------------------------------------------------------

bool is_input_valid(t_philo_data *data, size_t ac, char **av);

// ---- Allocate memory and initialize mutexes -------------------------------------------

int data_alloc(t_philo_data *data);
void data_free(t_philo_data *data, int destroy_mutex);

// ---- Initialize data ------------------------------------------------------------------

void data_init(t_philo_data *data);

// ---- Utils ----------------------------------------------------------------------------

int64_t get_timeofday_ms(pthread_mutex_t *lock_printf);
int print_safe(const char *err_msg, int exit_code, pthread_mutex_t *lock_printf);

// ---- Test functions -------------------------------------------------------------------

void test_print_data(t_philo_data *data, int buffer);

# endif