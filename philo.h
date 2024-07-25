/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dstinghe <dstinghe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:11:22 by castronela        #+#    #+#             */
/*   Updated: 2024/07/25 02:58:41 by dstinghe         ###   ########.fr       */
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
# include <stdint.h>
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
}	t_status_t;

typedef enum e_fork
{
	FORK,
	NO_FORK
}	t_fork;

typedef enum e_thread
{
	RUNNING,
	ENDED,
	STOPPED
}	t_thread;

typedef struct s_philo
{
	int philo_total;
	int philo_id;
	t_status_t	status;

	uint64_t	time_synch_start;
	int			time;
	int			time_start;
	int			time_event;
	int			time_eat_last;
	
	int			time_death;
	int			time_eat;
	int			time_sleep;
	int			time_think;
	int			eat_count;
	
	int			fork_count;
	t_fork		*fork_left;
	pthread_mutex_t	*lock_fork_left;
	t_fork		*fork_right;
	pthread_mutex_t	*lock_fork_right;
	
	t_thread	*thread;
	pthread_mutex_t *lock_thread;
	
	pthread_mutex_t *lock_printf;	
}	t_philo_t;

typedef struct data_s {
    int philo_total;
	uint64_t	time_synch_start;

	t_philo_t *philo;
	pthread_t *philo_thread;
	
	t_fork *forks;
	pthread_mutex_t *lock_forks;

	t_thread *thread_status;
	pthread_mutex_t *lock_thread;
	
	pthread_mutex_t *lock_printf;
}   t_data_t;

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