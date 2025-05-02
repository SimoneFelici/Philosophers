#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_data	t_data;

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
	bool			is_eating;
}					t_philosopher;

typedef struct s_data
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	bool			simulation_end;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printing;
	pthread_mutex_t	state_mutex;
	t_philosopher	*philosophers;
	long			start_time;
}	t_data;

void	cleanup(t_data *data);
long	get_current_time(void);

#endif
