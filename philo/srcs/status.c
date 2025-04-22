/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpichon <gpichon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:13:19 by gpichon           #+#    #+#             */
/*   Updated: 2025/04/22 16:50:34 by gpichon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_death(t_philo *philo, long current_time)
{
	long	time_since_meal;

	if (!philo || !philo->data)
		return (1);
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (1);
	}
	time_since_meal = current_time - philo->last_meal_time;
	if (time_since_meal > philo->data->time_to_die)
	{
		philo->data->someone_died = TRUE;
		pthread_mutex_unlock(&philo->data->death_mutex);
		print_status(philo, "died");
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (0);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;
	int		is_death;

	if (!philo || !philo->data || !status)
		return ;
	is_death = (status[0] == 'd' && status[1] == 'i'
			&& status[2] == 'e' && status[3] == 'd' && status[4] == '\0');
	pthread_mutex_lock(&philo->data->death_mutex);
	timestamp = get_time() - philo->data->start_time;
	if (is_death && philo->data->death_printed)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	if (is_death)
		philo->data->death_printed = TRUE;
	printf("%ld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->death_mutex);
}

int	should_continue(t_philo *philo)
{
	int	result;

	if (!philo || !philo->data)
		return (0);
	pthread_mutex_lock(&philo->data->death_mutex);
	result = !philo->data->someone_died;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (result);
}

static int	philosopher_cycle(t_philo *philo)
{
	if (!should_continue(philo))
		return (0);
	if (!philo_eat(philo))
		return (0);
	if (philo->data->meals_to_eat != -1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->meals_eaten >= philo->data->meals_to_eat)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			return (0);
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
	}
	if (!should_continue(philo))
		return (0);
	philo_sleep(philo);
	if (!should_continue(philo))
		return (0);
	philo_think(philo);
	return (1);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (philosopher_cycle(philo))
		;
	return (NULL);
}
