/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfelici <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:05:50 by sfelici           #+#    #+#             */
/*   Updated: 2025/05/03 18:05:51 by sfelici          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*eat(void *arg)
{
	t_philosopher	*phil;
	t_data			*data;
	long			now;

	phil = (t_philosopher *)arg;
	data = phil->data;
	while (true)
	{
		pthread_mutex_lock(&data->state_mutex);
		if (data->simulation_end)
		{
			pthread_mutex_unlock(&data->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->state_mutex);
		if (phil->id % 2 == 0)
		{
			pthread_mutex_lock(phil->left_fork);
			pthread_mutex_lock(&data->printing);
			printf("%ld %d has taken a fork\n", get_current_time() \
				- data->start_time, phil->id);
			pthread_mutex_unlock(&data->printing);
			pthread_mutex_lock(phil->right_fork);
			pthread_mutex_lock(&data->printing);
			printf("%ld %d has taken a fork\n", get_current_time() \
				- data->start_time, phil->id);
			pthread_mutex_unlock(&data->printing);
		}
		else
		{
			pthread_mutex_lock(phil->right_fork);
			pthread_mutex_lock(&data->printing);
			printf("%ld %d has taken a fork\n", get_current_time() \
				- data->start_time, phil->id);
			pthread_mutex_unlock(&data->printing);
			pthread_mutex_lock(phil->left_fork);
			pthread_mutex_lock(&data->printing);
			printf("%ld %d has taken a fork\n", get_current_time() \
				- data->start_time, phil->id);
			pthread_mutex_unlock(&data->printing);
		}
		pthread_mutex_lock(&data->state_mutex);
		if (data->simulation_end)
		{
			pthread_mutex_unlock(&data->state_mutex);
			pthread_mutex_unlock(phil->left_fork);
			pthread_mutex_unlock(phil->right_fork);
			break ;
		}
		pthread_mutex_unlock(&data->state_mutex);
		pthread_mutex_lock(&data->printing);
		now = get_current_time();
		printf("%ld %d is eating\n", now - data->start_time, phil->id);
		pthread_mutex_unlock(&data->printing);
		phil->is_eating = true;
		phil->last_meal_time = now;
		while (get_current_time() - now < data->time_to_eat)
			usleep(100);
		phil->meals_eaten++;
		phil->is_eating = false;
		pthread_mutex_unlock(phil->left_fork);
		pthread_mutex_unlock(phil->right_fork);
		pthread_mutex_lock(&data->state_mutex);
		if (data->simulation_end)
		{
			pthread_mutex_unlock(&data->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->state_mutex);
		pthread_mutex_lock(&data->printing);
		printf("%ld %d is sleeping\n", get_current_time() \
			- data->start_time, phil->id);
		pthread_mutex_unlock(&data->printing);
		now = get_current_time();
		while (get_current_time() - now < data->time_to_sleep)
			usleep(100);
		pthread_mutex_lock(&data->state_mutex);
		if (data->simulation_end)
		{
			pthread_mutex_unlock(&data->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->state_mutex);
		pthread_mutex_lock(&data->printing);
		printf("%ld %d is thinking\n", get_current_time() \
			- data->start_time, phil->id);
		pthread_mutex_unlock(&data->printing);
	}
	return (NULL);
}

static void	*monitor(void *arg)
{
	t_data	*data;
	int		i;
	bool	all_full;
	long	dt;

	data = (t_data *)arg;
	while (true)
	{
		i = 0;
		while (i < data->num_philosophers)
		{
			pthread_mutex_lock(&data->state_mutex);
			dt = get_current_time() - data->philosophers[i].last_meal_time;
			if (dt > data->time_to_die)
			{
				data->simulation_end = true;
				pthread_mutex_unlock(&data->state_mutex);
				pthread_mutex_lock(&data->printing);
				printf("%ld %d died\n", get_current_time() - data->start_time, \
					data->philosophers[i].id);
				pthread_mutex_unlock(&data->printing);
				return (NULL);
			}
			pthread_mutex_unlock(&data->state_mutex);
			i++;
		}
		if (data->num_meals > 0)
		{
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
				return (NULL);
			}
		}
		usleep(1000);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;
	int			i;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s num_philosophers time_to_die time_to_eat \
			time_to_sleep [num_meals]\n", argv[0]);
		return (1);
	}
	data.num_philosophers = atoi(argv[1]);
	data.time_to_die = atoi(argv[2]);
	data.time_to_eat = atoi(argv[3]);
	data.time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data.num_meals = atoi(argv[5]);
	else
		data.num_meals = 0;
	data.simulation_end = false;
	data.forks = malloc(sizeof(pthread_mutex_t) * data.num_philosophers);
	data.philosophers = malloc(sizeof(t_philosopher) * data.num_philosophers);
	pthread_mutex_init(&data.printing, NULL);
	pthread_mutex_init(&data.state_mutex, NULL);
	i = 0;
	while (i < data.num_philosophers)
	{
		pthread_mutex_init(&data.forks[i], NULL);
		i++;
	}
	data.start_time = get_current_time();
	i = 0;
	while (i < data.num_philosophers)
	{
		data.philosophers[i].id = i + 1;
		data.philosophers[i].meals_eaten = 0;
		data.philosophers[i].last_meal_time = data.start_time;
		data.philosophers[i].is_eating = false;
		data.philosophers[i].left_fork = &data.forks[i];
		data.philosophers[i].right_fork = &data.forks[(i + 1) \
			% data.num_philosophers];
		data.philosophers[i].data = &data;
		pthread_create(&data.philosophers[i].thread_id, NULL, \
			eat, &data.philosophers[i]);
		usleep(60);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor, &data);
	i = 0;
	while (i < data.num_philosophers)
	{
		pthread_join(data.philosophers[i].thread_id, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	cleanup(&data);
	return (0);
}
