#include "philosophers.h"

void	*watch_stat(void *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	int			i;

	p = (t_ph_prop *)args;
	phil = p->phil;
	i = 0;
	while (p->eat_num != p->total)
	{
		usleep(10);
		if (p->t_die < (int)(get_time() - p->start_t - phil[i].ate_stmp))
		{
			print_stat(p, &phil[i], STDEA);
			return (NULL);
		}
		++i;
		if (i == p->total)
			i = 0;
	}
	print_stat(p, phil, STFIN);
	return (NULL);
}

void	f_waiter_odds(int max, int total, int wait, t_phil *phil)
{
	int	counter;
	int	eating;

	counter = 0;
	eating = 1;
	while (eating)
	{
		pthread_mutex_unlock(&phil[eating - 1].wait_fork);
		if (++counter == max)
		{
			fsleep(wait);
			counter = 0;
		}
		if (eating + 1 == total)
			eating = 1;
		else if (eating == total)
			eating = 2;
		else
			eating += 2;
	}
}

void	f_waiter_evens(int max, int total, int wait, t_phil *phil)
{
	int	counter;
	int	eating;

	counter = 0;
	eating = 1;
	while (eating)
	{
		pthread_mutex_unlock(&phil[eating - 1].wait_fork);
		if (++counter == max)
		{
			fsleep(wait);
			counter = 0;
		}
		if (eating == total)
			eating = 1;
		else if (eating + 1 == total)
			eating = 2;
		else
			eating += 2;
	}
}

void	*f_waiter(void *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	int			max_eat;

	p = (t_ph_prop *)args;
	phil = p->phil;
	if (p->total % 2 != 0)
	{
		max_eat = (p->total - 1) / 2;
		f_waiter_odds(max_eat, p->total, p->t_eat, phil);
	}
	else
	{
		max_eat = p->total / 2;
		f_waiter_evens(max_eat, p->total, p->t_eat, phil);
	}
	return (NULL);
}

int	wait_phils(t_ph_prop *p)
{
	pthread_t	watcher;
	pthread_t	waiter;

	if (p->total != 1)
	{
		if (pthread_create(&waiter, NULL, &f_waiter, p))
			return (perr_exit("Pthread_create for waiter failed:"));
		if (pthread_detach(waiter))
			return (perr_exit("Pthread detach for waiter failed"));
	}
	if (pthread_create(&watcher, NULL, watch_stat, p))
		return (perr_exit("Pthread_create for watcher failed:"));
	if (pthread_join(watcher, NULL))
		return (perr_exit("Watcher could not join:"));
	return (0);
}
