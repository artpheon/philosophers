#include "philosophers.h"
//ms - milliseconds. 1 millisecond = 1000 microsenconds

t_phil *get_next_phil(t_ph_prop *p)
{
	t_phil *ph;

	++p->i;
	ph = p->phil;
	while (ph->id != p->i)
	{
		ph = ph->right;
	}
	return (ph);
}

void print_stat(t_ph_prop * const p, t_phil *phil, const int stat)
{
	pthread_mutex_lock(&p->print_lock);
	if (stat == 1)
		printf("%llu\t%d took fork\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 2)
		printf("%llu\t%d is eating\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 3)
		printf("%llu\t%d is sleeping\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 4)
		printf("%llu\t%d is thinking\n", get_time() - p->start_t, (int)phil->id);
	if (stat == 5)
	{
		printf("%llu\tsimulation finished!\n", get_time() - p->start_t);
		return ;
	}
	if (stat == 0)
	{
		printf("%llu\t%d is dead\n", get_time() - p->start_t, (int) phil->id);
		phil->st_dea = true;
		return ;
	}
	pthread_mutex_unlock(&p->print_lock);
}

void eat_routine_even(t_ph_prop *p, t_phil *phil)
{
	if (phil->lhanded)
	{
		pthread_mutex_lock(&phil->fork_left);
		print_stat(p, phil, STFRK);
		pthread_mutex_lock(phil->fork_right);
		print_stat(p, phil, STFRK);
	}
	else if (phil->rhanded)
	{
		pthread_mutex_lock(phil->fork_right);
		print_stat(p, phil, STFRK);
		pthread_mutex_lock(&phil->fork_left);
		print_stat(p, phil, STFRK);
	}
	phil->ate_stmp = get_time() - p->start_t;
	print_stat(p, phil, STEAT);
	fsleep(ms_to_micros(p->t_eat));
	pthread_mutex_unlock(&phil->fork_left);
	pthread_mutex_unlock(phil->fork_right);
}

void eat_routine_odd(t_ph_prop *p, t_phil *phil)
{
	pthread_mutex_lock(&p->fork_lock);
	pthread_mutex_lock(&phil->fork_left);
	print_stat(p, phil, STFRK);
	pthread_mutex_lock(phil->fork_right);
	print_stat(p, phil, STFRK);
	pthread_mutex_unlock(&p->fork_lock);
	phil->ate_stmp = get_time() - p->start_t;
	print_stat(p, phil, STEAT);
	fsleep(ms_to_micros(p->t_eat));
	pthread_mutex_unlock(&phil->fork_left);
	pthread_mutex_unlock(phil->fork_right);
}

void pcycle_eat_routine(t_ph_prop *p, t_phil *phil)
{
	if (p->total % 2 == 0)
		eat_routine_even(p, phil);
	else
		eat_routine_odd(p, phil);
}

void *pcycle(void *args)
{
	t_ph_prop	*p;
	t_phil		*phil;
	int i;

	p = (t_ph_prop *)args;
	phil = get_next_phil(p);
	i = 0;
	while (i < p->m_eat_num || p->m_eat_num == 0)
	{
		pcycle_eat_routine(p, phil);
		if (i == p->m_eat_num - 1)
			phil->fin = true;
		print_stat(p, phil, STSLP);
		fsleep(ms_to_micros(p->t_slp));
		print_stat(p, phil, STTHNK);
		++i;
	}
	return (NULL);
}

bool is_dead(t_ph_prop *p, t_phil *phil)
{
	if (get_time() - p->start_t - phil->ate_stmp >= p->t_die)
	{
		print_stat(p, phil, STDEA);
		return (true);
	}
	return (false);
}

bool watch_stat_finish(t_ph_prop *p)
{
	int i;
	t_phil *phil;

	i = 0;
	phil = p->phil;
	while (i < p->total)
	{
		if (phil->fin)
		{
			++i;
			phil = phil->right;
		}
		else
			return (false);
	}
	return (true);
}

void *watch_stat(void *args)
{
	t_ph_prop	*p;
	t_phil *phil;
	uint64_t now;

	p = (t_ph_prop *)args;
	phil = p->phil;
	now = get_time() - p->start_t;
	while (now - phil->ate_stmp < p->t_die)
	{
		usleep(10);
		now = get_time() - p->start_t;
		phil = phil->right;
		if (watch_stat_finish(p))
		{
			print_stat(p, phil, STFIN);
			return (NULL);
		}
	}
	print_stat(p, phil, STDEA);
	return (NULL);
}

int main(int argc, char *argv[])
{
	t_ph_prop p;
	pthread_t *threads;
	pthread_t watcher;
	size_t i;

	setbuf(stdout, NULL);
	if (ph_fill_prop(&p, argv, argc) == 1)
		return (1);
	threads = (pthread_t *)malloc(sizeof(pthread_t) * p.total);
	if (!threads)
		return (perr_exit("malloc failed."));
	i = 0;
	while (i < p.total)
	{
		if (pthread_create(&threads[i], NULL, pcycle, &p))
			return (perr_exit("philosopher thread creation failed."));
		//pthread_detach(threads[i]);
		++i;
	}
	if (pthread_create(&watcher, NULL, watch_stat, &p))
		return (perr_exit("watcher was not created."));
	i = 0;
	if (pthread_join(watcher, NULL) == 0)
		return (0);
	while (i < p.total)
	{
		pthread_join(threads[i], NULL);
		++i;
	}
	return(0);
}
