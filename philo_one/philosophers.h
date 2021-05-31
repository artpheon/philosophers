#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# define STFRK 1
# define STEAT 2
# define STSLP 3
# define STTHNK 4
# define STFIN 5
# define STDEA 0
# include <stdio.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_phil
{
	int				id;
	int				ate_num;
	bool			fin;
	uint64_t		ate_stmp;
	pthread_mutex_t	fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	wait_fork;
}	t_phil;

typedef struct s_ph_prop
{
	int				total;
	int				t_die;
	int				t_eat;
	int				t_slp;
	int				m_eat_num;
	int				eat_num;
	int				i;
	uint64_t		start_t;
	pthread_mutex_t	print_lock;
	t_phil			*phil;
}	t_ph_prop;

int			ft_atoi(const char *str);
int			ft_isdigit(int c);
int			is_strnum(const char *str);
int			err_exit(char *str);
int			perr_exit(char *str);
uint64_t	ms_to_micros(unsigned int ms);
uint64_t	get_time(void);
void		fsleep(useconds_t mcrsec);

int			phil_tonull(t_phil *p);
void		phil_set(t_phil *ph, int index, size_t size);
t_phil		*prop_get_phil(size_t n);
int			fill_prop_num(int *num, int argc, char *argv[]);
void		ph_fill_prop_basic(t_ph_prop *p, const int num[], int argc);
int			ph_fill_prop(t_ph_prop *p, char *argv[], int argc);

void		print_stat(t_ph_prop *p, t_phil *phil, int stat);
void		pcycle_eat_routine(t_ph_prop *p, t_phil *phil);
void		*pcycle(void *args);

void		*watch_stat(void *args);
void		f_waiter_odds(int max, int total, int wait, t_phil *phil);
void		f_waiter_evens(int max, int total, int wait, t_phil *phil);
void		*f_waiter(void *args);
int			wait_phils(t_ph_prop *p);

#endif
