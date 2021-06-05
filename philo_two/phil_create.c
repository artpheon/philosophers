#include "philosophers.h"

static void	set_sem_name(char name[], int num)
{
	int	i;

	if (num > 999)
		return ;
	name[0] = 's';
	name[1] = 'e';
	name[2] = 'm';
	i = 3;
	if (num == 0)
		name[i++] = '0';
	while (num != 0)
	{
		name[i] = (num % 10) - '0';
		num /= 10;
		++i;
	}
	name[i] = '\0';
}

t_phil	*prop_get_phil(int n)
{
	t_phil	*new;
	int		i;
	char	name[10];

	new = (t_phil *)malloc(sizeof(t_phil) * n);
	if (new)
	{
		i = 0;
		while (i < n)
		{
			new[i].id = i + 1;
			new[i].ate_stmp = 0;
			set_sem_name(name, i);
			new[i].eat_block = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
			if (new[i].eat_block == SEM_FAILED)
				return (NULL);
			if (sem_unlink(name))
				return (NULL);
			sem_wait(new[i].eat_block);
			++i;
		}
		return (new);
	}
	return (NULL);
}

int	fill_prop_num(int *num, int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc > 6 || argc < 5)
		return (err_exit("Wrong arguments number"));
	while (i < argc && argv[i])
	{
		if (is_strnum(argv[i]))
			return (err_exit("Argument is not a number"));
		num[i - 1] = ft_atoi(argv[i]);
		if (num[i - 1] < 0)
			return (err_exit("Invalid argument - negative number"));
		++i;
	}
	return (0);
}

int	ph_fill_prop_basic(t_ph_prop *p, const int num[], int argc)
{
	p->i = 0;
	p->eat_num = 0;
	p->total = num[0];
	p->t_die = num[1];
	p->t_eat = num[2];
	p->t_slp = num[3];
	p->start_t = 0;
	if (argc == 6)
		p->m_eat_num = num[4];
	else
		p->m_eat_num = 0;
	if (p->t_eat >= p->t_die || p->t_slp >= p->t_die)
		return (err_exit("Philosophers will die too quickly"));
	return (0);
}

int	fill_prop_sem(t_ph_prop *p)
{
	p->sem = sem_open("fork_sem", O_CREAT | O_EXCL, 0644, p->total);
	if (p->sem == SEM_FAILED)
		return (perr_exit("Semaphore failed"));
	if (sem_unlink("fork_sem"))
		return (perr_exit("Sem_unlink failed"));
	p->common_lock = sem_open("common_lock", O_CREAT | O_EXCL, 0644, 1);
	if (p->common_lock == SEM_FAILED)
		return (perr_exit("Semaphore failed"));
	if (sem_unlink("common_lock"))
		return (perr_exit("Sem_unlink failed"));
	return (0);
}
