#include "philo.h"

static void	take_left_fork(t_philosopher *phil, t_data *data)
{
	pthread_mutex_lock(phil->left_fork);
	pthread_mutex_lock(&data->printing);
	printf("%ld %d has taken a fork\n", get_current_time() \
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
}

static void	take_right_fork(t_philosopher *phil, t_data *data)
{
	pthread_mutex_lock(phil->right_fork);
	pthread_mutex_lock(&data->printing);
	printf("%ld %d has taken a fork\n", get_current_time() \
		- data->start_time, phil->id);
	pthread_mutex_unlock(&data->printing);
}

static void	take_forks(t_philosopher *phil, t_data *data)
{
	if (phil->id % 2 == 0)
	{
		take_left_fork(phil, data);
		take_right_fork(phil, data);
	}
	else
	{
		take_right_fork(phil, data);
		take_left_fork(phil, data);
	}
}

static void	release_forks(t_philosopher *phil)
{
	pthread_mutex_unlock(phil->left_fork);
	pthread_mutex_unlock(phil->right_fork);
}

static void	philosopher_eat(t_philosopher *phil, t_data *data)
{
	long	now;

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
}
