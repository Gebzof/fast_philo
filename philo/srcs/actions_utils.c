/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpichon <gpichon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 20:30:00 by gebz              #+#    #+#             */
/*   Updated: 2025/04/22 17:52:22 by gpichon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	print_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d died\n", get_time() - philo->data->start_time, philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

static int	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d is thinking\n", get_time() - philo->data->start_time,
		philo->id);
	pthread_mutex_unlock(&philo->data->print_mutex);
	smart_sleep(philo, philo->data->time_to_die);
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->data->someone_died = TRUE;
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->death_printed)
	{
		printf("%ld %d died\n", get_time() - philo->data->start_time,
			philo->id);
		philo->data->death_printed = TRUE;
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
	return (0);
}

int	check_death_status(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->data->death_mutex);
	result = philo->data->someone_died;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (result);
}

static int	lock_forks(t_philo *philo)
{
	if (check_death_status(philo))
		return (0);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[philo->left_fork]);
		pthread_mutex_lock(&philo->data->forks[philo->right_fork]);
	}
	return (1);
}

int	take_forks(t_philo *philo)
{
	if (!philo || !philo->data || !philo->data->forks)
		return (0);
	if (philo->data->num_philos == 1)
		return (handle_single_philo(philo));
	if (check_death_status(philo))
		return (0);
	if (!lock_forks(philo))
		return (0);
	if (check_death_status(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return (0);
	}
	if (!print_fork_taken(philo) || !print_fork_taken(philo))
	{
		pthread_mutex_unlock(&philo->data->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->data->forks[philo->left_fork]);
		return (0);
	}
	return (1);
}
