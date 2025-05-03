/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_states.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfelici <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:53:02 by sfelici           #+#    #+#             */
/*   Updated: 2025/05/03 18:53:04 by sfelici          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philosopher_sleep(t_philosopher *phil, t_data *data)
{
	long	now;

	pthread_mutex_lock(&data->printing);
	printf("%ld %d is sleeping\n", get_current_time() \
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
	now = get_current_time();
	while (get_current_time() - now < data->time_to_sleep)
		usleep(100);
}

void	philosopher_think(t_philosopher *phil, t_data *data)
{
	pthread_mutex_lock(&data->printing);
	printf("%ld %d is thinking\n", get_current_time() \
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
}

bool	check_simulation_end(t_data *data)
{
	bool	end;

	pthread_mutex_lock(&data->state_mutex);
	end = data->simulation_end;
	pthread_mutex_unlock(&data->state_mutex);
	return (end);
}

bool	check_philosopher_death(t_data *data, int i)
{
	long	dt;

	pthread_mutex_lock(&data->state_mutex);
	dt = get_current_time() - data->philosophers[i].last_meal_time;
	if (dt > data->time_to_die)
	{
		data->simulation_end = true;
		pthread_mutex_unlock(&data->state_mutex);
		pthread_mutex_lock(&data->printing);
		printf("%ld %d died\n", get_current_time() - data->start_time,
			data->philosophers[i].id);
		pthread_mutex_unlock(&data->printing);
		return (true);
	}
	pthread_mutex_unlock(&data->state_mutex);
	return (false);
}

bool	check_all_philosophers_full(t_data *data)
{
	int		i;
	bool	all_full;

	if (data->num_meals <= 0)
		return (false);
	all_full = true;
	i = 0;
	while (i < data->num_philosophers)
	{
		if (data->philosophers[i].meals_eaten < data->num_meals)
		{
			all_full = false;
			break ;
		}
		i++;
	}
	if (all_full)
	{
		data->simulation_end = true;
		return (true);
	}
	return (false);
}
