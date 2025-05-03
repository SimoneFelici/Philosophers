/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sfelici <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:53:33 by sfelici           #+#    #+#             */
/*   Updated: 2025/05/03 18:53:34 by sfelici          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	bool			is_eating;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_t		thread_id;
	struct s_data	*data;
}	t_philosopher;

typedef struct s_data
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	bool			simulation_end;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printing;
	pthread_mutex_t	state_mutex;
	t_philosopher	*philosophers;
}	t_data;

/* philo_actions.c */
void	take_forks(t_philosopher *phil, t_data *data);
void	release_forks(t_philosopher *phil);
void	philosopher_eat(t_philosopher *phil, t_data *data);

/* philo_states.c */
void	philosopher_sleep(t_philosopher *phil, t_data *data);
void	philosopher_think(t_philosopher *phil, t_data *data);
bool	check_simulation_end(t_data *data);
bool	check_philosopher_death(t_data *data, int i);
bool	check_all_philosophers_full(t_data *data);

/* philo_threads.c */
void	*eat(void *arg);
void	*monitor(void *arg);
void	initialize_philosophers(t_data *data);
void	wait_for_threads(t_data *data, pthread_t monitor_thread);

/* philo_init.c */
int		parse_arguments(int argc, char **argv, t_data *data);
int		initialize_data(t_data *data);

/* utils.c */
long	get_current_time(void);
void	cleanup(t_data *data);

#endif
