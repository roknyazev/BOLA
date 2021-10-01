#include <sys/socket.h>
#include <netinet/in.h>
#include "../ARINC429.h"
#include "../Timer.h"

#define PORT        12346
#define LOCALHOST   2130706433
#define N_ATTEMPTS  1000000000
#define DURATION    1ms


ssize_t read_data(int sock_fd)
{
	uint32_t buf;
	ARINC data{};
	ssize_t err;

	if ((err = recv(sock_fd, &buf, 4, 0)) == -1)
		return (err);
	data.raw = buf;
	std::cout << "ARINC429" << std::endl;

	printf("BCD\n"
		   "address: %15u\n"
		   "seconds: %15u\n"
		   "minutes: %15u\n"
		   "hours:   %15u\n"
		   "SM:      %15u\n"
		   "P:       %15u\n",
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
	struct sockaddr_in  server_address{};
	milliseconds        duration = DURATION;
	ssize_t             attempts = 0;
	ssize_t             err;

	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 )
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(LOCALHOST);
	server_address.sin_port = htons(PORT);
	if (bind(sock_fd, (const struct sockaddr *)(&server_address), sizeof(server_address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		err = timer<int>(read_data, sock_fd, duration);
		if (err == -1)
			attempts++;
		if (attempts >= N_ATTEMPTS)
			break;
	}
	return 0;
}