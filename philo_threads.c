/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfelici <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:53:11 by sfelici           #+#    #+#             */
/*   Updated: 2025/05/03 18:53:13 by sfelici          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*eat(void *arg)
{
	t_philosopher	*phil;
	t_data			*data;

	phil = (t_philosopher *)arg;
	data = phil->data;
	while (true)
	{
		if (check_simulation_end(data))
			break ;
		take_forks(phil, data);
		if (check_simulation_end(data))
		{
			release_forks(phil);
			break ;
		}
		philosopher_eat(phil, data);
		release_forks(phil);
		if (check_simulation_end(data))
			break ;
		philosopher_sleep(phil, data);
		if (check_simulation_end(data))
			break ;
		philosopher_think(phil, data);
	}
	return (NULL);
}

static void	*monitor(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (true)
	{
		i = 0;
		while (i < data->num_philosophers)
		{
			if (check_philosopher_death(data, i))
				return (NULL);
			i++;
		}
		if (check_all_philosophers_full(data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

static void	initialize_philosophers(t_data *data)
{
	int	i;

	data->start_time = get_current_time();
	i = 0;
	while (i < data->num_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].meals_eaten = 0;
		data->philosophers[i].last_meal_time = data->start_time;
		data->philosophers[i].is_eating = false;
		data->philosophers[i].left_fork = &data->forks[i];
		data->philosophers[i].right_fork = &data->forks[(i + 1) \
			% data->num_philosophers];
		data->philosophers[i].data = data;
		pthread_create(&data->philosophers[i].thread_id, NULL, \
			eat, &data->philosophers[i]);
		usleep(60);
		i++;
	}
}

static void	wait_for_threads(t_data *data, pthread_t monitor_thread)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}
