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

FILE *fINS;
FILE *fSNS;

ssize_t read_data(int sock_fd)
{
	ARINC data[13]{};
	ssize_t err;

	if ((err = recv(sock_fd, data, 13 * sizeof(ARINC), 0)) == -1)
		return (err);


	if (data[12].discrete.address == 184u)
	{
		fprintf(fINS, "\n"
			   "0 Широта:                     %15f\n"
			   "1 Долгота:                    %15f\n"
			   "2 Высота:                     %15f\n"
			   "3 Курс истинный:              %15f\n"
			   "4 Угол тангажа:               %15f\n"
			   "5 Угол крена:                 %15f\n"
			   "6 Скорость Север/Юг:          %15f\n"
			   "7 Скорость Восток/Запад:      %15f\n"
			   "8 Скорость вертикальная:      %15f\n"
			   "9 Ускорение продольное:       %15f\n"
			   "10 Ускорение поперечное:      %15f\n"
			   "11 Ускорение нормальное:      %15f\n"
			   "12.1 Адрес:                   %15u\n"
				"12.2 Подготовка:             %15u\n"
				"12.3 Исправность ИНС:        %15u\n"
				"12.4 Нет начальных данных:   %15u\n"
				"12.5 Готовность:             %15u\n",
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
			   data[12].discrete.address,
				data[12].discrete.preparation,
				data[12].discrete.INS_serviceability,
				data[12].discrete.no_initial_data,
				data[12].discrete.readiness);
	}
	else
	{
		fprintf(fSNS, "\n"
					  "0 Высота:              %15f\n"
					  "1 HDOP:                %15f\n"
					  "2 VDOP:                %15f\n"
					  "3 Путевой угол:        %15f\n"
					  "4 Текущая широта:      %15f\n"
					  "5 Текущая широта:      %15f\n"
					  "6 Текущая долгота:     %15f\n"
					  "7 Текущая широта:      %15f\n"
					  "8 Задержка выдачи НП:  %15f\n"
					  "9 :                    %15f\n"
					  "10 :                   %15f\n"
					  "11 :                   %15f\n"
					  "12.1:                  %15u\n"
					  "12.2:                  %15u\n"
					  "12.3:                  %15u\n",
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
				data[12].bcd.hours,
				data[12].bcd.minutes,
				data[12].bcd.seconds);
	}

	return (err);
}


int main()
{
	int                 sock_fd;
	struct sockaddr_in  server_address{};
	milliseconds        duration = DURATION;
	ssize_t             attempts = 0;
	ssize_t             err;

	fINS = fopen("ins.txt", "w");
	fSNS = fopen("sns.txt", "w");
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
	fclose(fINS);
	fclose(fSNS);
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