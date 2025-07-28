/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfelici <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:54:10 by sfelici           #+#    #+#             */
/*   Updated: 2025/05/03 18:54:12 by sfelici          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_left_fork(t_philosopher *phil, t_data *data)
{
	pthread_mutex_lock(phil->left_fork);
	if (check_simulation_end(data))
		return ;
	pthread_mutex_lock(&data->printing);
	printf("%ld %d has taken a fork\n", get_current_time()
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
}

static void	take_right_fork(t_philosopher *phil, t_data *data)
{
	pthread_mutex_lock(phil->right_fork);
	if (check_simulation_end(data))
		return ;
	pthread_mutex_lock(&data->printing);
	printf("%ld %d has taken a fork\n", get_current_time()
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
}

void	take_forks(t_philosopher *phil, t_data *data)
{
	if (data->num_philosophers == 1)
	{
		pthread_mutex_lock(phil->left_fork);
		pthread_mutex_lock(&data->printing);
		printf("%ld %d has taken a fork\n",
			get_current_time() - data->start_time, phil->id);
		pthread_mutex_unlock(&data->printing);
		while (!check_simulation_end(data))
			usleep(100);
		pthread_mutex_unlock(phil->left_fork);
		return ;
	}
	if (phil->id % 2 == 0)
	{
		take_left_fork(phil, data);
		if (!check_simulation_end(data))
			take_right_fork(phil, data);
	}
	else
	{
		take_right_fork(phil, data);
		if (!check_simulation_end(data))
			take_left_fork(phil, data);
	}
}

void	release_forks(t_philosopher *phil)
{
	pthread_mutex_unlock(phil->left_fork);
	if (phil->right_fork != phil->left_fork)
		pthread_mutex_unlock(phil->right_fork);
}

void	philosopher_eat(t_philosopher *phil, t_data *data)
{
	long	start_eating;

	start_eating = get_current_time();
	pthread_mutex_lock(&data->printing);
	printf("%ld %d is eating\n", start_eating - data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
	pthread_mutex_lock(&data->state_mutex);
	phil->is_eating = true;
	phil->last_meal_time = start_eating;
	phil->meals_eaten++;
	pthread_mutex_unlock(&data->state_mutex);
	while (get_current_time() - start_eating < data->time_to_eat)
	{
		if (check_simulation_end(data))
			break ;
		usleep(100);
	}
	pthread_mutex_lock(&data->state_mutex);
	phil->is_eating = false;
	pthread_mutex_unlock(&data->state_mutex);
}
