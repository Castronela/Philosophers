/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:11:22 by castronela        #+#    #+#             */
/*   Updated: 2024/07/25 06:45:31 by dstinghe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define ERROR_INPUT "Error - invalid input\n"
# define ERROR_MEM_ALLOC "Error - memory allocation\n"
# define ERROR_MUTEX_INIT "Error - mutex initialization\n"
# define ERROR_THREAD_LAUNCH "Error - thread launch\n"
# define ERROR_GETTIMEOFDAY "Error - gettimeofday\n"

# define INT_MAX_DIGIT_COUNT 10
# define SYNCH_DELAY_U_SEC 1000000

typedef enum e_status
{
	START,
	EAT,
	SLEEP,
	THINK,
	DEAD
}					t_status_t;

typedef enum e_fork
{
	FORK,
	NO_FORK
}					t_fork;

typedef enum e_thread
{
	RUNNING,
	ENDED,
	STOPPED
}					t_thread;

typedef struct s_philo
{
	int				philo_total;
	int				philo_id;
	t_status_t		status;

	uint64_t		time_synch_start;
	int				time;
	int				time_start;
	int				time_event;
	int				time_eat_last;

	int				time_death;
	int				time_eat;
	int				time_sleep;
	int				time_think;
	int				eat_count;

	int				fork_count;
	t_fork			*fork_left;
	pthread_mutex_t	*lock_fork_left;
	t_fork			*fork_right;
	pthread_mutex_t	*lock_fork_right;

	t_thread		*thread;
	pthread_mutex_t	*lock_thread;

	bool			*someone_died;
	pthread_mutex_t	*lock_printf;
}					t_philo_t;

typedef struct data_s
{
	int				philo_total;
	uint64_t		time_synch_start;

	t_philo_t		*philo;
	pthread_t		*philo_thread;

	t_fork			*forks;
	pthread_mutex_t	*lock_forks;

	t_thread		*thread_status;
	pthread_mutex_t	*lock_thread;

	bool			someone_died;
	pthread_mutex_t	lock_printf;
}					t_data_t;

int					main(int ac, char **av);

/** Input parsing **/
long long			ft_atoi(char *av);
void				init_input_data(t_data_t *data, int ac, int value);
void				init_data(t_data_t *data, int i);
int					setup_data(t_data_t *data);
int					get_time_start(t_data_t *data, int philo_id);
int					get_time_think(t_data_t *data);

/** Alloc memory **/
int					memalloc_data(t_data_t *data);
int					vars_allocmem(t_data_t *data);
int					mutex_allocmem(t_data_t *data);
void				clean(t_data_t *data);

/** Mutex handlers **/
int					init_mutex(t_data_t *data);
void				mutex_destroy(pthread_mutex_t *lock, int i);

/** Utils **/
bool				is_digit(int c);
bool				is_whitespace(int c);
bool				input_valid(char *av);
int					error(t_data_t *data, char *err_msg, int clean_all);
int					init_time(uint64_t *time);

/** Threads **/
void				thread_join(pthread_t *thread, int i);
int					thread_handler(t_data_t *data);
void				term_thread(t_data_t *data);
bool				term_check(t_data_t *data);
void				term_all(t_data_t *data);

/** Philo **/
void				*philo_thread(void *arg);
void				set_thread(t_philo_t *philo, t_thread thread);
bool				thread_check(t_philo_t *philo);
void				set_status(t_philo_t *philo, t_status_t status);
void				philo_iteration_latency(t_philo_t *philo);
void				get_forks(t_philo_t *philo);
void				reset_forks(t_philo_t *philo);
void				philo_synch(t_philo_t *philo);
void				put_stdout(t_philo_t *philo, char *action, int error);

#endif