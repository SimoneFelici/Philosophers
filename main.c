#include "philo.h"

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
