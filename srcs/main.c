#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <time.h>

#define ICMP_HEADER_SIZE 8
#define IP_HEADER_SIZE 20
#define DEFAULT_PAYLOAD_SIZE 56
#define PACKET_SIZE 64

int total_packet_size = IP_HEADER_SIZE + ICMP_HEADER_SIZE + DEFAULT_PAYLOAD_SIZE;

void setup_ip_address(char *hostname, char ip_address[INET_ADDRSTRLEN])
{
	struct addrinfo 	hints, *res;
	int 				status;
	struct sockaddr_in	*ipv4;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0)
	{
		fprintf(stderr, "ping: %s: %s\n", hostname, gai_strerror(status));
		exit(-1);
	}
	ipv4 = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &ipv4->sin_addr, ip_address, INET_ADDRSTRLEN);
	freeaddrinfo(res);
}

uint16_t calculate_checksum(void *b, int len)
{
	uint16_t *buf = b;
	uint32_t sum = 0;

	while (len > 1)
	{
		sum += *buf++;
		len -=2;
	}
	if (len == 1)
		sum += *(uint8_t *)buf;
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)(~sum);
}

void create_icmp_packet(struct icmphdr *icmp_package, int sequence)
{
	icmp_package->type = ICMP_ECHO;
	icmp_package->code = 0;
	icmp_package->un.echo.id = getpid();
	icmp_package->un.echo.sequence = sequence;
	icmp_package->checksum = calculate_checksum((uint16_t *)icmp_package, sizeof(struct icmphdr));
}

int send_ping(int sockfd, struct sockaddr_in *addr, const char *ip_address, int sequence)
{
	struct icmphdr		icmp_package;
	int 				ret;
    fd_set				readfds;
	double				elapsed;
    struct sockaddr_in	response_addr;
    socklen_t			addr_len = sizeof(response_addr);
    char				response[PACKET_SIZE];
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

	memset(&icmp_package, 0, sizeof(icmp_package));
	create_icmp_packet(&icmp_package, sequence);
	ret = sendto(sockfd, &icmp_package, sizeof(icmp_package), 0, (struct sockaddr *)addr, sizeof(*addr));
	if (ret < 0)
	{
		perror("sendto");
		return (-1);
	}
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
	ret = select(sockfd + 1, &readfds, NULL, NULL, NULL);
	if (ret > 0)
	{
		ret = recvfrom(sockfd, response, PACKET_SIZE, 0, (struct sockaddr *)&response_addr, &addr_len);
		if (ret < 0)
		{
			perror("recvfrom");
			exit(-1);
		}
            clock_gettime(CLOCK_MONOTONIC, &end_time);
		//getnameinfo() pour avoir le FQDN
    	elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                    (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
        printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n", PACKET_SIZE, "FQDN", ip_address, sequence, 0, elapsed);
        return 0;
	}
	return (-1);
}

int main(int ac, char **av)
{
	char	*hostname, ip_address[INET_ADDRSTRLEN];
	struct	sockaddr_in addr;
	int		sockfd, i;

	if (ac > 3)
	{
		fprintf(stderr, "\nUsage\n\tping [options] <destination>\n");
		return 1;
	}
	hostname = (ac == 2 ? av[1]: av[2]); 
	setup_ip_address(hostname, ip_address);
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = inet_addr(ip_address);
	if (addr.sin_addr.s_addr == INADDR_NONE)
	{
		fprintf(stderr, "Invalid IP address.\n");
		return 3;
	}
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0)
	{
		perror("socket");
		return 2;
	}
	printf("PING %s (%s) %d(%d) bytes of data.\n", hostname, ip_address, DEFAULT_PAYLOAD_SIZE, total_packet_size);
	printf("%ld\n", sizeof(uint16_t));
	i = 0;
	while (1)
	{
		if (send_ping(sockfd, &addr, ip_address, i) < 0)
		{
			fprintf(stderr, "Error sending ping\n");
		}
		sleep(1);
		i++;
	}
	close(sockfd);
	return 0;
}