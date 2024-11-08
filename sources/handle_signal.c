#include "ft_ping.h"

extern t_ping_data ping_data;

void sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		ping_data.percent_loss = (((float)ping_data.packet_send - (float)ping_data.packet_received)
									/ (float)ping_data.packet_send) * 100;
		ping_data.rtt_avg = ping_data.packet_received ? ping_data.rtt_sum 
														/ ping_data.packet_received : 0;
		ping_data.rtt_mdev = ping_data.packet_received ? sqrt((ping_data.rtt_sum_sq 
																/ ping_data.packet_received) 
																- (ping_data.rtt_avg 
																* ping_data.rtt_avg)) : 0;
	    printf("\n--- %s ft_ping statistics ---\n", ping_data.hostname);
		printf("%d packets transmitted, %d received, %.0f%% packet loss, time %.0fms\n",
					ping_data.packet_send, ping_data.packet_received, 
					ping_data.percent_loss, time_diff(ping_data.start_time, ping_data.end_time));
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", ping_data.rtt_min,
				ping_data.rtt_avg, ping_data.rtt_max, ping_data.rtt_mdev);
	}
    exit(0);
}

void	handle_signal(void)
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
        perror("Unable to set signal handler");
        exit(1);
    }
}
