#include "ft_ping.h"

extern int optind;

void parse_arg(t_ping_data *p_data, int argc, char **argv)
{
	int opt;

	while((opt = getopt(argc, argv, "?v")) != -1)
	{
		switch(opt)
		{
			case '?': p_data->options |= HELP_OPT; break;
			case 'v': p_data->options |= V_OPT; break;
			default:
				break;
		}
	}
	p_data->arg = *(argv + optind);
}