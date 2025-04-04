/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:58 by david             #+#    #+#             */
/*   Updated: 2025/02/04 14:19:25 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define USLEEP_VALUE 100
# define INT_MAX_DIGITS 10
# define START_DELAY_MS 1000

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DEAD "died"

# define ERRMSG_INPUT "\033[0;31mInvalid Input: \033[0m"
# define ERRMSG_ARG_FEW "Too few arguments:"
# define ERRMSG_ARG_MANY "Too many arguments:"
# define ERRMSG_NUM_INVALID "Invalid number:"
# define ERRMSG_NUM_LARGE "Argument size larger than INT_MAX:"
# define ERRMSG_NUM_NEG "Number must be positive:"

# define ERRMSG_INTERNAL "\033[0;31mInternal Error: \033[0m"
# define ERRMSG_MALLOC "malloc"
# define ERRMSG_MUTEX_INIT "pthread_mutex_init"
# define ERRMSG_MUTEX_DESTR "pthread_mutex_destroy"
# define ERRMSG_MUTEX_LOCK "pthread_mutex_lock"
# define ERRMSG_MUTEX_UNLOCK "pthread_mutex_unlock"
# define ERRMSG_TIMEOFDAY "timeofday"
# define ERRMSG_THREAD_CR "pthread_create"
# define ERRMSG_THREAD_JO "pthread_join"

typedef enum e_invalid_input_type
{
	VALID,
	ARGS_TOO_FEW,
	ARGS_TOO_MANY,
	INVALID,
	TOO_LARGE,
	NEGATIVE
}					t_invalid_input_type;

typedef enum e_state
{
	RUNNING,
	DEAD,
	STOPPED,
	EXITED,
	INT_FAIL
}					t_state;

typedef enum e_action
{
	THINK,
	EAT,
	SLEEP,
	DIED
}					t_action;

typedef struct s_philos
{
	uint64_t		time_start_ms;
	t_state			status;
	pthread_mutex_t	lock_status;
	unsigned int	philo_id;

	unsigned int	time_ms;
	unsigned int	time_last_eat;

	unsigned int	time_die;
	unsigned int	time_eat;
	unsigned int	time_sleep;
	int				eat_count;

	unsigned int	*fork_left;
	pthread_mutex_t	*lock_fork_left;
	unsigned int	fork_right;
	pthread_mutex_t	lock_fork_right;
	unsigned int	fork_count;

	bool			*stop_printf;
	pthread_mutex_t	*lock_printf;

	pthread_t		thread;
}					t_philos;

typedef struct s_philo_data
{
	uint64_t		time_start_ms;

	unsigned int	philo_count;
	unsigned int	time_die;
	unsigned int	time_eat;
	unsigned int	time_sleep;
	int				eat_count;

	t_philos		*philos;

	bool			stop_printf;
	pthread_mutex_t	lock_printf;
}					t_philo_data;

int					main(int ac, char **av);

// ---- Input validation -------------------------------------------------------

bool				is_input_valid(t_philo_data *data, size_t ac, char **av);

// ---- Allocate memory and initialize mutexes ---------------------------------

int					data_init(t_philo_data *data);
void				data_free(t_philo_data *data, const int destroy_mutex);

// ---- Initialize data --------------------------------------------------------

void				philos_init(t_philo_data *data, t_philos *philos,
						size_t index);

// ---- Threads ----------------------------------------------------------------

int					threads_main(t_philo_data *data);

// ---- Monitor ----------------------------------------------------------------

int					threads_monitor(t_philo_data *data);
void				threads_stop(t_philo_data *data);

// ---- Philosopher threads ----------------------------------------------------

void				*philo_thread(void *philo_void);

int					apply_sleep(t_philos *philo, const unsigned int time_ms);
int					check_stopped_thread(t_philos *philo,
						uint64_t current_time_ms);
bool				thread_stop(t_philos *philo, const t_state change_status);

// ---- Fork actions -----------------------------------------------------------

int					forks_pickup(t_philos *philo);
int					forks_putback(t_philos *philo);
int					check_stopped_thread(t_philos *philo,
						uint64_t current_time_ms);
bool				is_stopped(t_philos *philo, const t_state change_status);

// ---- Utils ------------------------------------------------------------------

int					update_time(t_philos *philo, uint64_t current_time_ms);
int64_t				get_timeofday_ms(void);
int					print_safe(t_philos *philo, t_action action);

#endif