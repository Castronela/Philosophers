/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: david <david@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:58 by david             #+#    #+#             */
/*   Updated: 2025/01/05 18:06:32 by david            ###   ########.fr       */
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
# include <errno.h>

# define INT_MAX_DIGITS 10
# define START_DELAY_MS 1000

# define COLOR_RED "\033[0;31m"
# define COLOR_RESET "\033[0m"

# define ERRMSG_INPUT COLOR_RED"Input Invalid Error: "COLOR_RESET
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
# define ERRMSG_THREAD_CR "pthread_create"
# define ERRMSG_THREAD_JO "pthread_join"

typedef enum e_invalid_input_type {
    VALID,
    ARGS_TOO_FEW,     
    ARGS_TOO_MANY,   
    INVALID,
    TOO_LARGE,
    NEGATIVE
}   t_invalid_input_type;

typedef enum e_state {
    RUNNING,
    DEAD,
    STOPPED,    // status for a thread that must stop due to death or internal func failure of other thread
    EXITED,     // status for a thread that has finished executing without problems
    INT_FAIL    // status for a function failure
}   t_state;

typedef struct s_philos {
    uint64_t time_start_ms;
    unsigned int philo_id;
    unsigned int philo_count;
    
    unsigned int time_ms;
    unsigned int time_last_activity;
    unsigned int time_last_eat;
    
    unsigned int time_die;
    unsigned int time_eat;
    unsigned int time_sleep;
    unsigned int time_think;
    int eat_count;

    t_state status;
    unsigned int *fork_left;
    pthread_mutex_t *lock_fork_left;
    unsigned int *fork_right;
    pthread_mutex_t *lock_fork_right;
    unsigned int fork_count;
    
    pthread_mutex_t *lock_printf;
    bool *stop_threads;
}   t_philos;

typedef struct s_philo_data {
    uint64_t time_start_ms;
    
    unsigned int philo_count;
    unsigned int time_die;
    unsigned int time_eat;
    unsigned int time_sleep;
    int eat_count;

    t_philos *philos;
    pthread_t *threads;
    
    bool stop_threads;
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

// ---- Threads --------------------------------------------------------------------------

int threads_main(t_philo_data *data);

// ---- Philosopher threads --------------------------------------------------------------

void *philo_thread(void *philo_void);

// ---- Utils ----------------------------------------------------------------------------

int update_time(t_philos *philo);
int64_t get_timeofday_ms(const bool safe, t_philos *philo);
void print_safe(t_philos *philo, const bool is_philo_state, const char *msg);

// ---- Test functions -------------------------------------------------------------------

void test_print_data(t_philo_data *data, int buffer);
void test_print_exit_status(t_philos *philo);

# endif