#include <sys/socket.h>
#include <netinet/in.h>
#include "Ins.h"

#define SERVER_PORT         12346
#define SERVER_ADDRESS      2130706433 //localhost
#define CLIENT_PORT         12344
#define CLIENT_ADDRESS      2130706433 //localhost
#define N_ATTEMPTS          100
#define DURATION            1000ms


typedef struct			arg
{
	int					sock_fd;
	struct sockaddr_in	server_address;
	std::mutex			*m;
	Ins 				*ins;

}              			arg_t;


ssize_t send_data(arg_t args)
{
	int sock_fd = args.sock_fd;
	struct sockaddr_in server_address = args.server_address;
	ssize_t err;

	args.m->lock();
	err = sendto(sock_fd,
				 (args.ins->packed_state),
				 (13 * sizeof(ARINC)),
				 0,
				 (const struct sockaddr *) &server_address, sizeof(server_address));
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
	Ins 				*ins;
	Aircraft			*aircraft;
	auto *checkpoints = new std::list<double *>;
	auto tmp1 = new double[2];
	auto tmp2 = new double[2];
	auto tmp3 = new double[2];


	tmp1[0] = 5600.0;
	tmp1[1] = 5600.0;
	checkpoints->push_back(tmp1);
	tmp2[0] = -5500.744307;
	tmp2[1] = 3700.409644;
	checkpoints->push_back(tmp2);
	tmp3[0] = 5500.599954;
	tmp3[1] = -3700.218756;
	checkpoints->push_back(tmp3);

	aircraft = new Aircraft(0, 0, 0, 0, 100, checkpoints);

	m = new std::mutex();
	ins = new Ins(m, 2ms, 1000ms, 1000ms, aircraft);

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
	arg.ins = ins;
    if (bind(sock_fd, (const struct sockaddr *)(&client_address), sizeof(client_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	std::thread	thread1(ins_work, std::ref(*ins));
	std::thread	thread2(start_aircraft, std::ref(*aircraft));
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