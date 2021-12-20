#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include "bin.h"
#include "protocol.h"
#include <chrono>
#include <time.h>
#include <stdio.h>
#include <iomanip>
#include <winsock2.h>
#include <iostream>
//#include <windows.h>

#pragma comment(lib, "ws2_32.lib")



#define PORT 12346    // порт сервера
#pragma warning(disable: 4996)

using namespace std::chrono_literals;
using namespace std;

int main() {

	char buff[4];

	printf("UDP DEMO echo-Server\n");

	// шаг 1 - подключение библиотеки 
	if (WSAStartup(0x202, (WSADATA*)& buff[0]))
	{
		printf("WSAStartup error: %d\n",
			WSAGetLastError());
		return -1;
	}

	// шаг 2 - создание сокета
	SOCKET my_sock;
	my_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (my_sock == INVALID_SOCKET)
	{
		printf("Socket() error: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// шаг 3 - связывание сокета с локальным адресом 
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = INADDR_ANY;
	local_addr.sin_port = htons(PORT);

	if (bind(my_sock, (sockaddr*)& local_addr,
		sizeof(local_addr)))
	{
		printf("bind error: %d\n", WSAGetLastError());
		closesocket(my_sock);
		WSACleanup();
		return -1;
	}

	FILE* ofile;
	const char* name = "result.txt";
	fopen_s(&ofile, name, "w");		// файл с результатом
	const char* f_header = "%-34s %-34s %-25s";
	fprintf_s(ofile, f_header, "Время приема сообщения", "Имя параметра", "Значение");

	int i;
	int x1 = bindec(table[111].max_value, 20, table[111].height, 80.321);
	double x2 = massdec(table[111].max_value, 20, table[111].height, x1);
	printf("x1 = %ld\nreturn = %f", x1, x2);
	// шаг 4 обработка пакетов, присланных клиентами
    while (1)
	{
		sockaddr_in client_addr;
		int client_addr_size = sizeof(client_addr);
		int bsize = recvfrom(my_sock, &buff[0],
			sizeof(buff), 0,
			(sockaddr*)&client_addr, &client_addr_size);

		if (bsize == SOCKET_ERROR)
			printf("recvfrom() error: %d\n",
				WSAGetLastError());
		
		ARINC_BNR pack1;
		
		memcpy(&pack1, buff, sizeof(pack1));
		printf("recvfrom()%d\n", pack1.bits.adress);

		

		if (table[pack1.bits.adress].name == "слово БИНС")
		{
			ARINC_DISCRETE pack1;
			memcpy(&pack1, buff, sizeof(pack1));


			SYSTEMTIME st;

			GetLocalTime(&st);

			fprintf(ofile, "\n%-4d-%02d-%02d %02d:%02d:%02d.%-15d",
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds);

			char* cstr = new char[table[pack1.bits.adress].name.length() + 1];
			strcpy(cstr, table[pack1.bits.adress].name.c_str());
			fprintf_s(ofile, "%-35s", cstr);
			fprintf_s(ofile, "%-10.10s", " ");

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "SDI");
			fprintf_s(ofile, "%-35d", pack1.bits.SDI);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Подготовка по ЗК");
			fprintf_s(ofile, "%-35d", pack1.bits.preparation);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Контроль");
			fprintf_s(ofile, "%-35d", pack1.bits.control);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Навигация");
			fprintf_s(ofile, "%-35d", pack1.bits.navigation);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Гирокомпассирование");
			fprintf_s(ofile, "%-35d", pack1.bits.gyro);

			//fprintf_s(ofile, "\n%-35s", "  ");

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Повторный запуск");
			fprintf_s(ofile, "%-35d", pack1.bits.re_start);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Шкала подготовки");
			fprintf_s(ofile, "%-35d", pack1.bits.scale);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Исправность обогрева");
			fprintf_s(ofile, "%-35d", pack1.bits.heat);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Термостатирование");
			fprintf_s(ofile, "%-35d", pack1.bits.therm);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Нет начальных данных");
			fprintf_s(ofile, "%-35d", pack1.bits.init_data);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Нет приема Набс");
			fprintf_s(ofile, "%-35d", pack1.bits.rec_H);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Исправность ИНС");
			fprintf_s(ofile, "%-35d", pack1.bits.INS);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Готовность ускоренная");
			fprintf_s(ofile, "%-35d", pack1.bits.acceleration);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Готовность");
			fprintf_s(ofile, "%-35d", pack1.bits.ready);

			//fprintf_s(ofile, "\n%-35s", "  ");

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "SM");
			fprintf_s(ofile, "%-35d", pack1.bits.SM);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "P");
			fprintf_s(ofile, "%-35d\n", pack1.bits.P);
		}

		if (table[pack1.bits.adress].name == "слово СНС")
		{
			ARINC_FEATURES pack1;
			memcpy(&pack1, buff, sizeof(pack1));


			SYSTEMTIME st;

			GetLocalTime(&st);

			fprintf(ofile, "\n%-4d-%02d-%02d %02d:%02d:%02d.%-15d",
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds);

			char* cstr = new char[table[pack1.bits.adress].name.length() + 1];
			strcpy(cstr, table[pack1.bits.adress].name.c_str());
			fprintf_s(ofile, "%-35s", cstr);
			fprintf_s(ofile, "%-10.10s", " ");

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Запрос начальных данных");
			fprintf_s(ofile, "%-35d", pack1.bits.R_data);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Тип рабочей СРНС");
			fprintf_s(ofile, "%-35d", pack1.bits.type_sns);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Альманах GPS");
			fprintf_s(ofile, "%-35d", pack1.bits.almanach_gps);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Альманах ГЛОНАСС");
			fprintf_s(ofile, "%-35d", pack1.bits.almanach_glo);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Режим работы");
			fprintf_s(ofile, "%-35d", pack1.bits.mode);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Подрежимы работы");
			fprintf_s(ofile, "%-35d", pack1.bits.submode);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Признак времени");
			fprintf_s(ofile, "%-35d", pack1.bits.time_f);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Диф. режим измерений");
			fprintf_s(ofile, "%-35d", pack1.bits.diff);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Отказ изделия");
			fprintf_s(ofile, "%-35d", pack1.bits.reject);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Порог сигнализации");
			fprintf_s(ofile, "%-35d", pack1.bits.signal_step);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Система координат");
			fprintf_s(ofile, "%-35d", pack1.bits.SK);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "SM");
			fprintf_s(ofile, "%-35d", pack1.bits.SM);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "P");
			fprintf_s(ofile, "%-35d\n", pack1.bits.P);
		}


		if (table[pack1.bits.adress].name != "слово СНС" && table[pack1.bits.adress].name != "слово БИНС" && table[pack1.bits.adress].name != "дата")
		{

			SYSTEMTIME st;

			GetLocalTime(&st);

			fprintf(ofile, "\n%-4d-%02d-%02d %02d:%02d:%02d.%-15d",
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds);


			char* cstr = new char[table[pack1.bits.adress].name.length() + 1];
			strcpy(cstr, table[pack1.bits.adress].name.c_str());
			fprintf_s(ofile, "%-35s", cstr);
			std::cout << pack1.bits.value;

			


			double decvalue = massdec(table[pack1.bits.adress].max_value, 20, table[pack1.bits.adress].height, pack1.bits.value);
			fprintf_s(ofile, "%-10.10f", decvalue);


		}



		if (table[pack1.bits.adress].name == "дата") {

			ARINC_DATE pack1;
			memcpy(&pack1, buff, sizeof(pack1));

			SYSTEMTIME st;

			GetLocalTime(&st);

			fprintf(ofile, "\n%-4d-%02d-%02d %02d:%02d:%02d.%-15d",
				st.wYear,
				st.wMonth,
				st.wDay,
				st.wHour,
				st.wMinute,
				st.wSecond,
				st.wMilliseconds);

			char* cstr = new char[table[pack1.bits.adress].name.length() + 1];
			strcpy(cstr, table[pack1.bits.adress].name.c_str());
			fprintf_s(ofile, "%-35s", cstr);
			fprintf_s(ofile, "%-10.10f", pack1.Value);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Год");
			fprintf_s(ofile, "%-35d", pack1.bits.Y_u);

			fprintf_s(ofile, "\n%-35s", " ");
			fprintf_s(ofile, "%-35s", " ");
			fprintf_s(ofile, "%-35d", pack1.bits.Y_d);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "Месяц");
			fprintf_s(ofile, "%-35d", pack1.bits.M_u);

			fprintf_s(ofile, "\n%-35s", " ");
			fprintf_s(ofile, "%-35s", " ");
			fprintf_s(ofile, "%-35d", pack1.bits.M_d);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "День");
			fprintf_s(ofile, "%-35d", pack1.bits.D_u);

			fprintf_s(ofile, "\n%-35s", " ");
			fprintf_s(ofile, "%-35s", " ");
			fprintf_s(ofile, "%-35d", pack1.bits.D_d);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "SM");
			fprintf_s(ofile, "%-35d", pack1.bits.SM);

			fprintf_s(ofile, "\n%-35s", "  ");
			fprintf_s(ofile, "%-35s", "P");
			fprintf_s(ofile, "%-35d\n", pack1.bits.P);


		}

	}


}

