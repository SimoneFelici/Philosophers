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
	long	start_sleep;

	pthread_mutex_lock(&data->printing);
	printf("%ld %d is sleeping\n", get_current_time()
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
	start_sleep = get_current_time();
	while (get_current_time() - start_sleep < data->time_to_sleep)
	{
		if (check_simulation_end(data))
			break ;
		usleep(100);
	}
}

void	philosopher_think(t_philosopher *phil, t_data *data)
{
	pthread_mutex_lock(&data->printing);
	printf("%ld %d is thinking\n", get_current_time()
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
	usleep(100);
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
	long	current_time;
	long	time_since_last_meal;
	bool	is_eating;

	current_time = get_current_time();
	pthread_mutex_lock(&data->state_mutex);
	time_since_last_meal = current_time - data->philosophers[i].last_meal_time;
	is_eating = data->philosophers[i].is_eating;
	if (is_eating)
	{
		pthread_mutex_unlock(&data->state_mutex);
		return (false);
	}
	if (time_since_last_meal >= data->time_to_die)
	{
		data->simulation_end = true;
		pthread_mutex_unlock(&data->state_mutex);
		pthread_mutex_lock(&data->printing);
		printf("%ld %d died\n", current_time - data->start_time,
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
	pthread_mutex_lock(&data->state_mutex);
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
		data->simulation_end = true;
	pthread_mutex_unlock(&data->state_mutex);
	return (all_full);
}
