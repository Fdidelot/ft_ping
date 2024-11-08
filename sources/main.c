#include "ft_ping.h"

int total_packet_size = IP_HEADER_SIZE + ICMP_HEADER_SIZE + DEFAULT_PAYLOAD_SIZE;
t_ping_data ping_data;

void send_icmp_package(void)
{
	struct icmphdr		icmp_package;
	int 				ret;

	memset(&icmp_package, 0, sizeof(icmp_package));
	create_icmp_packet(&icmp_package, ping_data.sequence);
	ret = sendto(ping_data.sockfd, &icmp_package, sizeof(icmp_package), 0, 
							(struct sockaddr *)&ping_data.addr, sizeof(ping_data.addr));
	if (ret < 0)
	{
		perror("sendto");
		exit(EXIT_FAILURE);
	}
}

int	wait_response(void)
{
	int					ret;
	ssize_t				packet_length;
    fd_set				readfds;
    struct sockaddr_in	response_addr;
    socklen_t			addr_len = sizeof(response_addr);

    FD_ZERO(&readfds);
    FD_SET(ping_data.sockfd, &readfds);
	ret = select(ping_data.sockfd + 1, &readfds, NULL, NULL, NULL);
	if (ret > 0)
	{
		packet_length = recvfrom(ping_data.sockfd, ping_data.response, PACKET_SIZE, 0,
						(struct sockaddr *)&response_addr, &addr_len);
		if (packet_length < 0)
		{
			perror("recvfrom");
			exit(-1);
		}
		return (0);
	}
	return (-1);
}

int send_ping(const char *ip_address, int sequence)
{
	int					ret;
	double				rtt;
    struct timespec 	start_time, end_time;
    struct iphdr 		*ip_header;

    clock_gettime(CLOCK_MONOTONIC, &start_time);
	send_icmp_package();
	ping_data.packet_send += 1;
	ret = wait_response();
    clock_gettime(CLOCK_MONOTONIC, &end_time);
	ping_data.end_time = end_time;
	if (ret == 0)
	{
		ping_data.packet_received += 1;
		ip_header = (struct iphdr *)ping_data.response;
    	rtt = time_diff(start_time, end_time);
		if (ping_data.rtt_min > rtt) ping_data.rtt_min = rtt; 
		if (ping_data.rtt_max < rtt) ping_data.rtt_max = rtt;
		ping_data.rtt_sum += rtt;
		ping_data.rtt_sum_sq += rtt * rtt;
		if (ping_data.only_addr == false)
	        printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n", 
					PACKET_SIZE, ping_data.fqdn, ip_address, sequence, ip_header->ttl, rtt);
		else
	        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", 
					PACKET_SIZE, ping_data.fqdn, sequence, ip_header->ttl, rtt);

        return (0);
	}
	return (-1);
}

int main(int ac, char **av)
{
	handle_signal();
	if (ac > 3 || ac < 2)
	{
		fprintf(stderr, "\nUsage\n\tping [options] <destination>\n");
		return(EXIT_FAILURE);
	}
	memset(&ping_data, 0, sizeof(ping_data));
	ping_data.hostname = (ac == 2 ? av[1]: av[2]);
	ping_data.sequence = 1;
	ping_data.rtt_min = 1e9;
	ping_data.only_addr = false;
	set_sockaddr_struct(&ping_data.addr);
	setup_ip_address(ping_data.hostname);
	if (ping_data.addr.sin_addr.s_addr == INADDR_NONE)
	{
		fprintf(stderr, "Invalid IP address.\n");
		return(EXIT_FAILURE);
	}
	ping_data.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping_data.sockfd < 0)
	{
		perror("socket");
		return(EXIT_FAILURE);
	}
	/* */
	printf("////\nhostname = %s\nip_address = %s\nfqdn = %s\n/////\n", ping_data.hostname, ping_data.ip_address, ping_data.fqdn);
	/* */
	if (strcmp(ping_data.hostname, ping_data.ip_address) == 0)
	{
		ping_data.only_addr = true;
		strcpy(ping_data.fqdn, ping_data.ip_address);
	}
	printf("PING %s (%s) %d(%d) bytes of data.\n", ping_data.hostname, ping_data.ip_address, 
													DEFAULT_PAYLOAD_SIZE, total_packet_size);
    clock_gettime(CLOCK_MONOTONIC, &ping_data.start_time);
	while (1)
	{
		if (send_ping(ping_data.ip_address, ping_data.sequence) < 0)
		{
			fprintf(stderr, "Error sending ping\n");
		}
		sleep(1);
		ping_data.sequence++;
	}
	close(ping_data.sockfd);
	return (0);
}