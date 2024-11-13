#include "ft_ping.h"

void	print_verbose_info(t_ping_data p_data)
{
	printf("ft_ping: sock4.fd: %d (socktype: SOCK_RAW), sock6.fd: -1 (socktype: 0), hints.ai_family: AF_INET\n\n", 
				p_data.sockfd);
}

void	print_usage_error(void)
{
	fprintf(stderr, "ft_ping: usage error: Destination address required\n");
	exit(EXIT_FAILURE);
}

void	print_start_info(t_ping_data p_data)
{
	if (p_data.options & V_OPT)
		printf("ai->ai_family: AF_INET, ai->canonname: '%s'\n", p_data.canonname);
	printf("PING %s (%s) %d(%d) bytes of data.\n", p_data.arg, p_data.ip_address, 
													DEFAULT_PAYLOAD_SIZE, TOTAL_PACKET_SIZE);
}

void	print_helper(void)
{
	printf("\nUsage\n  ft_ping [options] <destination>\n\n");
	printf("Options:\n  <destination>      DNS name or IP address\n");
	printf("  -v                 verbose output\n");
}

void	print_ping_info(t_ping_data *p_data, int ttl)
{
	printf("%d bytes from ", PACKET_SIZE);
	if (p_data->direct_ip)
		printf("%s", p_data->ip_address);
	else
		printf("%s (%s)", p_data->fqdn, p_data->ip_address);
	printf(": icmp_seq=%d ", p_data->sequence);
	if (p_data->options & V_OPT)
		printf("ident=%d ", p_data->pid);
	printf("ttl=%d ", ttl);
	if (p_data->rtt < 1.00)
		printf("time=%.3f ms\n", p_data->rtt);
	else
		printf("time=%.2f ms\n", p_data->rtt);
}

void	print_end_stats(t_ping_data p_data)
{
	printf("%d packets transmitted, %d received, %.0f%% packet loss, time %.0fms\n",
				p_data.packet_send, p_data.packet_received, 
				p_data.percent_loss, p_data.total_time);
	if (p_data.packet_received != 0)
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms", p_data.rtt_min,
				p_data.rtt_avg, p_data.rtt_max, p_data.rtt_mdev);
	printf("\n");
}