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
	printf("\n"
		   "0:  %15u\n"
		   "1:  %15u\n"
		   "2:  %15u\n"
		   "3:  %15u\n"
		   "4:  %15u\n"
		   "5:  %15u\n"
		   "6:  %15u\n"
		   "7:  %15u\n"
		   "8:  %15u\n"
		   "9:  %15u\n"
		   "10: %15u\n"
		   "11: %15u\n"
		   "12: %15u\n",
		   args.ins->packed_state[0].bnr.height,
		   args.ins->packed_state[1].bnr.height,
		   args.ins->packed_state[2].bnr.height,
		   args.ins->packed_state[3].bnr.height,
		   args.ins->packed_state[4].bnr.height,
		   args.ins->packed_state[5].bnr.height,
		   args.ins->packed_state[6].bnr.height,
		   args.ins->packed_state[7].bnr.height,
		   args.ins->packed_state[8].bnr.height,
		   args.ins->packed_state[9].bnr.height,
		   args.ins->packed_state[10].bnr.height,
		   args.ins->packed_state[11].bnr.height,
		   args.ins->packed_state[12].discrete.address);

	args.m->lock();
	err = sendto(sock_fd, (args.ins->packed_state), (13 * sizeof(ARINC)), 0, (const struct sockaddr *) &server_address, sizeof(server_address));
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

	m = new std::mutex();
	ins = new Ins(m, 2ms, 1000ms, 1000ms);

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
	std::thread	thread(ins_work, std::ref(*ins));
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