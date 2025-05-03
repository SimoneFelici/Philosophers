#include "philo.h"

static int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s num_philosophers time_to_die time_to_eat \
			time_to_sleep [num_meals]\n", argv[0]);
		return (1);
	}
	data->num_philosophers = atoi(argv[1]);
	data->time_to_die = atoi(argv[2]);
	data->time_to_eat = atoi(argv[3]);
	data->time_to_sleep = atoi(argv[4]);
	if (argc == 6)
		data->num_meals = atoi(argv[5]);
	else
		data->num_meals = 0;
	return (0);
}

static int	initialize_data(t_data *data)
{
	int	i;

	data->simulation_end = false;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
	data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);
	if (!data->forks || !data->philosophers)
		return (1);
	pthread_mutex_init(&data->printing, NULL);
	pthread_mutex_init(&data->state_mutex, NULL);
	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;

	if (parse_arguments(argc, argv, &data))
		return (1);
	if (initialize_data(&data))
		return (1);
	initialize_philosophers(&data);
	pthread_create(&monitor_thread, NULL, monitor, &data);
	wait_for_threads(&data, monitor_thread);
	cleanup(&data);
	return (0);
}
