/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpichon <gpichon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:55:08 by gebz              #+#    #+#             */
/*   Updated: 2025/04/22 17:28:15 by gpichon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_all_philosophers_ate(t_data *data)
{
	int	i;
	int	all_ate_enough;

	i = 0;
	all_ate_enough = 1;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->meals_to_eat != -1
			&& data->philos[i].meals_eaten < data->meals_to_eat)
			all_ate_enough = 0;
		pthread_mutex_unlock(&data->death_mutex);
		i++;
	}
	return (all_ate_enough);
}

void	clean_up(t_data *data)
{
	int	i;

	if (data->philos)
		free(data->philos);
	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->print_mutex);
}

int	print_fork_taken(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (check_death_status(philo))
		return (pthread_mutex_unlock(&philo->data->print_mutex), 0);
	printf("%ld %d has taken a fork\n",
		get_time() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	return (1);
}
