#ifndef FT_PING_H
# define FT_PING_H

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
#include <signal.h>
#include <math.h>
#include <stdbool.h>

#define ICMP_HEADER_SIZE 8
#define IP_HEADER_SIZE 20
#define DEFAULT_PAYLOAD_SIZE 56
#define PACKET_SIZE 64
#define LOOPBACK_ADDRESS "127.0.0.1"

typedef struct	s_ping_data
{
	char			*hostname;
	char			ip_address[INET_ADDRSTRLEN];
	struct			sockaddr_in addr;
	int				sockfd;
	int				sequence;
	char			fqdn[NI_MAXHOST];
	char			response[PACKET_SIZE];
	int				packet_send;
	int				packet_received;
	double			percent_loss;
	double			rtt_min;
	double			rtt_max;
	double			rtt_sum;
	double			rtt_sum_sq;
	double			rtt_avg;
	double			rtt_mdev;
	bool			only_addr;
	struct timespec	start_time;
	struct timespec	end_time;
}				t_ping_data;

void 		setup_ip_address(char *hostname);
void 		set_sockaddr_struct(struct sockaddr_in *addr);
uint16_t 	calculate_checksum(void *b, int len);
void		create_icmp_packet(struct icmphdr *icmp_package, int sequence);
void		handle_signal(void);
void		sigint_handler(int sig);
double		time_diff(struct timespec start_time, struct timespec end_time);

#endif