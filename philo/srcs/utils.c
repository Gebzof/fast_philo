/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gebz <gebz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 11:13:19 by gpichon           #+#    #+#             */
/*   Updated: 2025/04/23 01:52:45 by gebz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(t_philo *philo, long duration)
{
	long	start;
	long	current;
	long	elapsed;
	long	remaining;

	start = get_time();
	while (1)
	{
		current = get_time();
		if (check_death(philo, current))
			return ;
		elapsed = current - start;
		if (elapsed >= duration)
			break ;
		remaining = duration - elapsed;
		if (remaining >= 500)
			usleep(500);
		if (remaining >= 300)
			usleep(300);
		else if (remaining >= 100)
			usleep(100);
		else
			usleep(remaining);
	}
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (!check_args(argc, argv))
		return (1);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->meals_to_eat = -1;
	if (argc == 6)
		data->meals_to_eat = ft_atoi(argv[5]);
	data->someone_died = FALSE;
	data->death_printed = FALSE;
	data->philos = NULL;
	data->forks = NULL;
	data->start_time = get_time();
	return (0);
}
