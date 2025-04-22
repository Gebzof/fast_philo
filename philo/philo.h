/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpichon <gpichon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:13:19 by gpichon           #+#    #+#             */
/*   Updated: 2025/04/22 17:16:57 by gpichon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define TRUE 1
# define FALSE 0

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_to_eat;
	int				someone_died;
	int				death_printed;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	print_mutex;
	t_philo			*philos;
	pthread_t		watch_death;
};

struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals_eaten;
	long			last_meal_time;
	t_data			*data;
	pthread_t		thread;
};

/* actions.c */
int		philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

/* actions_utils.c */
int		take_forks(t_philo *philo);
void	release_forks(t_philo *philo);

/* time.c */
long	get_time(void);
void	smart_sleep(t_philo *philo, long duration);
int		should_continue(t_philo *philo);

/* status.c */
int		check_death(t_philo *philo, long current_time);
void	print_status(t_philo *philo, char *status);

/* init.c */
int		init_data(t_data *data, int argc, char **argv);
int		init_mutexes(t_data *data);
int		init_philosophers(t_data *data);
void	clean_up(t_data *data);
int		check_args(int argc, char **argv);

/* utils.c */
int		ft_atoi(const char *str);

/* philo.c */
void	*philosopher_routine(void *arg);

int		check_all_philosophers_ate(t_data *data);
int		check_death_status(t_philo *philo);
int		print_fork_taken(t_philo *philo);

#endif