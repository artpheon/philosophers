#include "philosophers.h"

void	print_stat(t_ph_prop *p, t_phil *phil, const int stat)
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
		return ;
	}
	pthread_mutex_unlock(&p->print_lock);
}

void	pcycle_eat_routine(t_ph_prop *p, t_phil *phil)
{
	pthread_mutex_lock(&phil->wait_fork);
	pthread_mutex_lock(&phil->fork_left);
	print_stat(p, phil, STFRK);
	pthread_mutex_lock(phil->fork_right);
	print_stat(p, phil, STFRK);
	print_stat(p, phil, STEAT);
	phil->ate_stmp = get_time() - p->start_t;
	fsleep(ms_to_micros(p->t_eat));
	phil->ate_num += 1;
	if (phil->ate_num == p->m_eat_num)
	{
		phil->fin = true;
		p->eat_num += 1;
	}
	pthread_mutex_unlock(&phil->fork_left);
	pthread_mutex_unlock(phil->fork_right);
}
