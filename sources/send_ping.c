#include "ft_ping.h"

void	send_icmp_package(t_ping_data *p_data)
{
	struct icmphdr	icmp_package;
	int				ret;

	memset(&icmp_package, 0, sizeof(icmp_package));
	set_icmp_package(&icmp_package, p_data->sequence);
	ret = sendto(p_data->sockfd, &icmp_package, sizeof(icmp_package), 0,
					(struct sockaddr *)&p_data->addr, sizeof(p_data->addr));
	if (ret < 0)
	{
		perror("sento");
		close(p_data->sockfd);
		exit(EXIT_FAILURE);
	}
	p_data->packet_send += 1;
}

int		wait_response(t_ping_data *p_data)
{
	int					ret;
	ssize_t				packet_lenght;
	fd_set				readfds;
	struct sockaddr_in	response_addr;
	socklen_t			addr_len;
	struct timeval		timeout;

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	addr_len = sizeof(response_addr);
	FD_ZERO(&readfds);
	FD_SET(p_data->sockfd, &readfds);
	ret = select(p_data->sockfd + 1, &readfds, NULL, NULL, &timeout);
	if (ret < 0)
	{
		perror("select");
		close(p_data->sockfd);
		exit(EXIT_FAILURE);
	}
	if (ret == 0)
	{
		p_data->timeout = true;
	}
	else
	{
		packet_lenght = recvfrom(p_data->sockfd, p_data->response, PACKET_SIZE, 0,
									(struct sockaddr *)&response_addr, &addr_len);
		if (packet_lenght < 0)
		{
			perror("recvfrom");
			close(p_data->sockfd);
			exit(EXIT_FAILURE);
		}
		p_data->packet_received += 1;
		return (0);
	}
	return (-1);
}

void	send_ping(t_ping_data *p_data)
{
	struct timespec	start_time, end_time;
	struct iphdr	*ip_header;
	int				ret;

    clock_gettime(CLOCK_MONOTONIC, &start_time);
	send_icmp_package(p_data);
	ret = wait_response(p_data);
    clock_gettime(CLOCK_MONOTONIC, &end_time);
	p_data->end_time = end_time;
	if (ret == 0)
	{
		ip_header = (struct iphdr *)p_data->response;
		store_time_info(p_data, start_time, end_time);
		print_ping_info(p_data, ip_header->ttl);
	}
}