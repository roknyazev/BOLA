#include <sys/socket.h>
#include <netinet/in.h>
#include "../ARINC429.h"
#include "../Timer.h"
#include "../transform.h"
#include "bitset"


#define PORT        12346
#define ADDRESS   	2130706433 //localhost
#define N_ATTEMPTS  1000000000
#define DURATION    1ms



ssize_t read_data(int sock_fd)
{
	ARINC data[13]{};
	ssize_t err;

	if ((err = recv(sock_fd, data, 13 * sizeof(ARINC), 0)) == -1)
		return (err);

	std::cout << "ARINC429" << std::endl;

	printf("\n"
		   "0:  %15f\n"
		   "1:  %15f\n"
		   "2:  %15f\n"
		   "3:  %15f\n"
		   "4:  %15f\n"
		   "5:  %15f\n"
		   "6:  %15f\n"
		   "7:  %15f\n"
		   "8:  %15f\n"
		   "9:  %15f\n"
		   "10: %15f\n"
		   "11: %15f\n"
		   "12: %15u\n",
		   reverse_transform(20, data[0]),
		   reverse_transform(20, data[1]),
		   reverse_transform(20, data[2]),
		   reverse_transform(20, data[3]),
		   reverse_transform(20, data[4]),
		   reverse_transform(20, data[5]),
		   reverse_transform(20, data[6]),
		   reverse_transform(20, data[7]),
		   reverse_transform(20, data[8]),
		   reverse_transform(20, data[9]),
		   reverse_transform(20, data[10]),
		   reverse_transform(20, data[11]),
		   data[12].discrete.address);
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
	server_address.sin_addr.s_addr = htonl(ADDRESS);
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


//	ARINC test = {};
//	conversion nmb = {};

//	nmb.float_nmb = RemoveBits(12345.12345, pow(2, 12));
//	std::cout << nmb.float_nmb << std::endl;
//	std::bitset<32> n1(nmb.raw);
//	std::cout << n1 << std::endl;
//	test.bnr.height = nmb.raw >> 12;
//	nmb = {};
//	nmb.raw = test.bnr.height << 12;
//	std::cout << nmb.float_nmb << std::endl;
//	std::bitset<32> n2(nmb.raw);
//	std::cout << n2 << std::endl;