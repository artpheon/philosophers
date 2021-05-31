#include "philosophers.h"

void	phil_tonull(t_phil *p)
{
	p->left = NULL;
	p->right = NULL;
	p->st_eat = false;
	p->st_slp = false;
	p->st_thk = false;
	p->st_dea = false;
	p->fin = false;
	p->ate_stmp = 0;
	p->id = 0;
}

t_phil	*get_phil(t_phil *first, unsigned int n)
{
	t_phil	*prev;
	t_phil	*new;
	int	i;

	i = 1;
	prev = first;
	while (i < n)
	{
		new = (t_phil *)malloc(sizeof(t_phil));
		if (new == NULL)
			return (NULL);
		phil_tonull(new);
		new->id = (++i);
		prev->right = new;
		new->left = prev;
		prev = new;
	}
	return (new);
}

t_phil  *prop_get_phil(unsigned int n)
{
	t_phil	*first;
	t_phil	*last;

	first = NULL;
	last = NULL;
	if (n == 0)
		return (NULL);
	first = (t_phil *)malloc(sizeof(t_phil));
	if (first == NULL)
		return (NULL);
	phil_tonull(first);
	first->id = 1;
	if (n != 1)
	{
		last = get_phil(first, n);
		first->left = last;
		last->right = first;
	}
	return (first);
}

int fill_prop_num(long int *num, int argc, char *argv[])
{
	int i;

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

void	fill_prop_basic(t_ph_prop *p, int argc, const long int num[])
{
	p->i = 0;
	p->total = num[0];
	p->t_die = num[1];
	p->t_eat = num[2];
	p->t_slp = num[3];
	if (argc == 6)
		p->m_eat_num = num[4];
	else
		p->m_eat_num = -1;
	p->phil = prop_get_phil(p->total);
	p->stat = (int *)malloc(sizeof(int) * p->total);
	p->pid = (pid_t *)malloc(sizeof(pid_t) * p->total);
}

void	fill_prop_sem(t_ph_prop *p)
{
	p->sem = sem_open("sem", O_CREAT | O_EXCL, 0644, p->total);
	if (p->sem == SEM_FAILED)
		perror("semaphore failed.");
	if (sem_unlink("sem") != 0)
		perror("sem_unlink failed");
	p->common = sem_open("sem_common", O_CREAT | O_EXCL, 0644, 1);
	if (p->sem == SEM_FAILED)
		perror("semaphore failed.");
	if (sem_unlink("sem_common") != 0)
		perror("sem_unlink failed");
}

int	ph_fill_prop(t_ph_prop *p, char *argv[], int argc)
{
	long int num[5];

	if (fill_prop_num(num, argc, argv) == 1)
		return (1);
	if (p->total == 1)
		return (err_exit("Cannot be less than 2 philosophers"));
	if (p->t_eat >= p->t_die || p->t_slp >= p->t_die)
		return (err_exit("Philosophers will die before finishing eating or sleeping"));
	fill_prop_sem(p);
	fill_prop_basic(p, argc, num);
	if (p->phil == NULL)
		return (err_exit("Could not create a philosopher"));
	if (!p->stat || !p->pid)
		return (err_exit("malloc failed."));
	p->stat[0] = 0;
	p->pid[0] = 0;
	p->start_t = 0;
	return (0);
}