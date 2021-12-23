#include <sys/socket.h>
#include <netinet/in.h>
#include "Sns.h"

#define SERVER_PORT         12346
#define SERVER_ADDRESS      2130706433 //localhost
#define CLIENT_PORT         12345
#define CLIENT_ADDRESS      2130706433 //localhost
#define N_ATTEMPTS          100
#define DURATION            1000ms


typedef struct arg
{
	int sock_fd;
	struct sockaddr_in server_address;
	std::mutex			*m;
	Sns 				*sns;
}               arg_t;


ssize_t send_data(arg_t args)
{
	int sock_fd = args.sock_fd;
	struct sockaddr_in server_address = args.server_address;
	ssize_t err;

	args.m->lock();
	err = sendto(sock_fd, (args.sns->packed_state), (13 * sizeof(ARINC)), 0, (const struct sockaddr *) &server_address, sizeof(server_address));
	args.m->unlock();
	if (err == -1)
		return (err);
	return (err);
}


int main()
{
    int                 sock_fd;
    struct sockaddr_in  client_address{};
    struct sockaddr_in  server_address{};
    milliseconds        duration = DURATION;
    ssize_t             attempts = 0;
    ssize_t             err;
    arg_t               arg;
	std::mutex			*m;
	Sns 				*sns;

	m = new std::mutex();
	sns = new Sns(m, 4ms, 2000ms, 2000ms);

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 )
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = htonl(CLIENT_ADDRESS);
	client_address.sin_port = htons(CLIENT_PORT);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(SERVER_ADDRESS);
	server_address.sin_port = htons(SERVER_PORT);

	arg.sock_fd = sock_fd;
	arg.server_address = server_address;
	arg.m = m;
	arg.sns = sns;
	if (bind(sock_fd, (const struct sockaddr *)(&client_address), sizeof(client_address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	std::thread	thread(sns_work, std::ref(*sns));
	while (true)
	{
		err = timer<arg_t>(send_data, arg, duration);
		if (err == -1)
		{
			attempts++;
			std::cout << "try " << attempts << std::endl;
		}
		if (attempts >= N_ATTEMPTS)
			break;
	}
    return 0;
}