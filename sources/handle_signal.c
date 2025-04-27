#include "ft_ping.h"

extern t_ping_data p_data;

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		calculate_time_stats(&p_data);
	    printf("--- %s ft_ping statistics ---\n", p_data.arg);
		print_end_stats(p_data);
	}
	close(p_data.sockfd);
	exit(EXIT_SUCCESS);
}

void	handle_signal(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		perror("Unable to set signal handler");
		close(p_data.sockfd);
		exit(EXIT_FAILURE);
	}
}