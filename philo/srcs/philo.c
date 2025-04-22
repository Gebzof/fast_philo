/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpichon <gpichon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:13:16 by gebz              #+#    #+#             */
/*   Updated: 2025/04/22 17:31:56 by gpichon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	check_philosopher_death(t_data *data, int i, long current_time)
{
	int	should_die;

	pthread_mutex_lock(&data->death_mutex);
	should_die = (current_time - data->philos[i].last_meal_time
			> data->time_to_die);
	if (should_die)
	{
		data->someone_died = TRUE;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->print_mutex);
		if (!data->death_printed)
		{
			printf("%ld %d died\n", current_time - data->start_time,
				data->philos[i].id);
			data->death_printed = TRUE;
		}
		return (pthread_mutex_unlock(&data->print_mutex), 1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

void	*monitor_death(void *arg)
{
	t_data	*data;
	int		i;
	long	current_time;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			current_time = get_time();
			pthread_mutex_lock(&data->death_mutex);
			if (data->someone_died)
				return (pthread_mutex_unlock(&data->death_mutex), NULL);
			pthread_mutex_unlock(&data->death_mutex);
			if (check_philosopher_death(data, i, current_time))
				return (NULL);
			i++;
		}
		if (data->meals_to_eat != -1 && check_all_philosophers_ate(data))
			return (NULL);
		usleep(200);
	}
	return (NULL);
}

static int	create_threads(t_data *data)
{
	int			i;
	t_philo		*philo;

	i = 0;
	data->start_time = get_time();
	pthread_create(&data->watch_death, NULL, monitor_death, data);
	while (i < data->num_philos)
	{
		philo = &data->philos[i];
		if (pthread_create(&philo->thread, NULL, philosopher_routine, philo))
			return (1);
		i++;
	}
	return (0);
}

static int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			return (1);
		i++;
	}
	if (pthread_join(data->watch_death, NULL))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;

	if (!check_args(argc, argv) || argc < 5 || argc > 6)
		return (printf("Error: Invalid arguments\n"), 1);
	if (init_data(&data, argc, argv))
		return (printf("Error: Initialization failed\n"), 1);
	if (init_mutexes(&data))
	{
		printf("Error: Mutex initialization failed\n");
		return (clean_up(&data), 1);
	}
	if (init_philosophers(&data)
		|| create_threads(&data) || join_threads(&data))
	{
		printf("Error: Simulation failed\n");
		return (clean_up(&data), 1);
	}
	return (clean_up(&data), 0);
}
