#ifndef FT_PING_H
# define FT_PING_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <math.h>

#define ICMP_HEADER_SIZE 8
#define IP_HEADER_SIZE 20
#define DEFAULT_PAYLOAD_SIZE 56
#define TOTAL_PACKET_SIZE ICMP_HEADER_SIZE + IP_HEADER_SIZE + DEFAULT_PAYLOAD_SIZE
#define PACKET_SIZE 64
#define LOOPBACK_ADDRESS "127.0.0.1"
#define MAX_SIZE_NAME 128

enum
{
	HELP_OPT = 1 << 0,
	V_OPT = 1 << 1
};

typedef struct	s_ping_data
{
	unsigned int		options;
	int					pid;
	bool				direct_ip;
	bool				timeout;
	char				*arg;
	char				ip_address[INET_ADDRSTRLEN];
	char				fqdn[NI_MAXHOST];
	char				canonname[MAX_SIZE_NAME];
	char				response[PACKET_SIZE];
	struct sockaddr_in	addr;
	int					sockfd;
	unsigned int		sequence;
	double				rtt;
	double				rtt_min;
	double				rtt_max;
	double				rtt_sum;
	double				rtt_sum_sq;
	double				rtt_avg;
	double				rtt_mdev;
	double				percent_loss;
	double				total_time;
	unsigned int		packet_send;
	unsigned int		packet_received;
	struct timespec		start_time;
	struct timespec		end_time;
}				t_ping_data;

void	parse_arg(t_ping_data *p_data, int argc, char **argv);
void	set_p_data(t_ping_data *p_data);
void	get_ip_info(t_ping_data *p_data, char *ip);
void	send_ping(t_ping_data *p_data);
void	set_icmp_package(struct icmphdr *icmp_package, unsigned int sequence);
void	store_time_info(t_ping_data *p_data, struct timespec start_time,
												struct timespec end_time);
void	print_ping_info(t_ping_data *p_data, int ttl);
void	handle_signal(void);
void	calculate_time_stats(t_ping_data *p_data);
void	print_end_stats(t_ping_data p_data);
double	time_diff(struct timespec start_time, struct timespec end_time);
void	print_helper(void);
void	print_start_info(t_ping_data p_data);
void	print_usage_error(void);
void	print_verbose_info(t_ping_data p_data);

#endif