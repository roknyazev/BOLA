#include <sys/socket.h>
#include <netinet/in.h>
#include <random>
#include "../ARINC429.h"
#include "../Timer.h"

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
}               arg_t;


ssize_t send_data(arg_t args)
{
	int sock_fd = args.sock_fd;
	struct sockaddr_in server_address = args.server_address;
	std::default_random_engine generator(time(nullptr));
	std::uniform_int_distribution<int> distribution(0, INT32_MAX);
	uint32_t buf = distribution(generator);
	ssize_t err;
	ARINC data{};

	err = sendto(sock_fd, &buf, 4, 0, (const struct sockaddr *) &server_address, sizeof(server_address));
	if (err == -1)
		return (err);

	data.raw = buf;
	std::cout << "ARINC429" << std::endl;
	printf("BCD\n"
		   "address: %15u\n"
		   "seconds: %15u\n"
		   "minutes: %15u\n"
		   "hours:   %15u\n"
		   "SM:      %15u\n"
		   "P:       %15u\n\n",
		   data.bcd.address,
		   data.bcd.seconds,
		   data.bcd.minutes,
		   data.bcd.hours,
		   data.bcd.SM,
		   data.bcd.P);
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
    if (bind(sock_fd, (const struct sockaddr *)(&client_address), sizeof(client_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

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