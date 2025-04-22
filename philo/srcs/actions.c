/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gebz <gebz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:13:19 by gpichon           #+#    #+#             */
/*   Updated: 2025/04/23 01:51:57 by gebz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	release_forks(t_philo *philo)
{
	if (!philo || !philo->data || !philo->data->forks)
		return ;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
	}
}

static int	update_meal_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		release_forks(philo);
		return (0);
	}
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (1);
}

int	philo_eat(t_philo *philo)
{
	if (!philo || !philo->data)
		return (0);
	if (!take_forks(philo))
		return (0);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (check_death_status(philo))
		return (pthread_mutex_unlock(&philo->data->print_mutex), 0);
	print_status(philo, "is eating");
	pthread_mutex_unlock(&philo->data->print_mutex);
	if (!update_meal_status(philo))
		return (release_forks(philo), 0);
	smart_sleep(philo, philo->data->time_to_eat);
	release_forks(philo);
	return (1);
}

void	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (check_death_status(philo))
		return (pthread_mutex_unlock(&philo->data->print_mutex), (void)0);
	print_status(philo, "is sleeping");
	pthread_mutex_unlock(&philo->data->print_mutex);
	smart_sleep(philo, philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (check_death_status(philo))
		return (pthread_mutex_unlock(&philo->data->print_mutex), (void)0);
	print_status(philo, "is thinking");
	usleep(50);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
