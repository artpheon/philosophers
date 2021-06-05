#include "philosophers.h"

void	print_stat(t_ph_prop *p, t_phil *phil, int stat)
{
	sem_wait(p->common_lock);
	if (stat == 1)
		printf("%llu\t%d took fork\n", get_time() - p->start_t, phil->id);
	if (stat == 2)
		printf("%llu\t%d is eating\n", get_time() - p->start_t, phil->id);
	if (stat == 3)
		printf("%llu\t%d is sleeping\n", get_time() - p->start_t, phil->id);
	if (stat == 4)
		printf("%llu\t%d is thinking\n", get_time() - p->start_t, phil->id);
	if (stat == 5)
	{
		printf("%llu\tsimulation finished!\n", get_time() - p->start_t);
		return ;
	}
	if (stat == 0)
	{
		printf("%llu\t%d is dead\n", get_time() - p->start_t, phil->id);
		return ;
	}
	sem_post(p->common_lock);
}

void	pcycle_eat_routine(t_ph_prop *p, t_phil *phil, int round)
{
	sem_wait(phil->eat_block);
	sem_wait(p->sem);
	print_stat(p, phil, STFRK);
	sem_wait(p->sem);
	print_stat(p, phil, STFRK);
	print_stat(p, phil, STEAT);
	phil->ate_stmp = get_time() - p->start_t;
	fsleep(ms_to_micros(p->t_eat));
	sem_post(p->sem);
	sem_post(p->sem);
	if (round == p->m_eat_num - 1)
		p->eat_num += 1;
}
