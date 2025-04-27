#include "ft_ping.h"

t_ping_data p_data;

int main(int argc, char **argv)
{
	handle_signal();
	memset(&p_data, 0, sizeof(p_data));
	if (argc == 1)
		print_usage_error();
	parse_arg(&p_data, argc, argv);
	if (p_data.options & HELP_OPT)
	{
		print_helper();
		exit(EXIT_SUCCESS);
	}
	set_p_data(&p_data);
	// if (p_data.options & V_OPT)
	// 	print_verbose_info(p_data);
	get_ip_info(&p_data, p_data.arg);
	if (p_data.arg == NULL)
	{
		close(p_data.sockfd);
		print_usage_error();
	}
	print_start_info(p_data);
    clock_gettime(CLOCK_MONOTONIC, &p_data.start_time);
	while (1)
	{
		send_ping(&p_data);
		if (p_data.timeout == false)
			sleep(1);
		p_data.timeout = false;
		p_data.sequence += 1;
	}
	return 0;
}