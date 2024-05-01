#include<iostream>
#include<WinSock2.h>
#include<Ws2tcpip.h>
#include<fstream>
#include<string>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;


string Formatirovanie (string &Zapros)
{
	for (int i = 0; i < Zapros.size(); ++i)
	{   

		
		int marker = Zapros.find(",", i);
		if (marker != string::npos)
		{
			Zapros.replace(marker, 1, "\n");
			
		}
	}

	return Zapros;
}

string Formatirovanie2(string& Zapros)
{

	for (int i = 0; i < Zapros.size(); ++i)
	{
		int marker = Zapros.find("}", i);
		int marker1 = Zapros.find("{", i);
		int marker2 = Zapros.find("[", i);
		int marker3 = Zapros.find("]", i);
		int marker4=Zapros.find("\"", i );
		
		
		if (marker != string::npos)
		{
			Zapros.replace(marker, 1, " ");
		}
		if (marker1 != string::npos)
		{
			Zapros.replace(marker1, 1, " ");
		}

		if (marker2 != string::npos)
		{
			Zapros.replace(marker2, 1, " ");
		}

		if (marker3 != string::npos)
		{
			Zapros.replace(marker3, 1, " ");
		}

		if (marker4 != string::npos)
		{
			Zapros.replace(marker4, 1, " ");
		}
	}

	return Zapros;
}

string Razrab()
{

	return "Приложение разработал Даниил Куликов. тел. 89236349458. Спасибо за внимание. Программа сделана чтобы показать навыки работы с Winsock2";
}

void Info()
{
	cout << "В этой программе происходит соединение с сервером погоды, используя сокеты," << endl;
	cout << "потом принимается ответ в виде длинной строки полной дополнительных символов," << endl;
	cout << "и уже потом, часть символов заменяется на пробелы и ставится знак нового абзаца." << endl;
	cout << "Потом я записываю отформатированную строку в файл txt с названием Погода." << endl;
	cout << endl;
	cout << endl;
	cout << "Погода сегодня в Кемерово: " << endl;
	cout << endl;
	cout << endl;
}


int main()
{
	setlocale(LC_ALL, "ru");

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "Библиотека не инициализирована" << endl;
		return 1;
	}

	SOCKET firstSocket = socket(AF_INET, SOCK_STREAM, 0);

	addrinfo hints;
	addrinfo* serv;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo("api.openweathermap.org", "80", &hints, &serv);


	sockaddr_in AdressServera;

	AdressServera.sin_family = serv->ai_family;
	AdressServera.sin_port = htons(80);
	AdressServera.sin_addr = ((sockaddr_in*)serv->ai_addr)->sin_addr;


	if (connect(firstSocket, (const sockaddr*)&AdressServera, sizeof(AdressServera)) == SOCKET_ERROR)
	{
		cout << "Нет соединения с сервером" << endl;
		closesocket(firstSocket);
		WSACleanup();
		return 1;
	}

	string request = "GET /data/2.5/weather?q=Kemerovo&appid=c2175da694099d6d37442f69ad60c8c4&lang=ru/1.1\r\nHost: api.openweathermap.org\r\nConnection: close\r\n\r\n";

	send(firstSocket, request.c_str(), request.size(), 0);

	char buffer[1024];
	int indikator;
	string stroka;

	while ((indikator = recv(firstSocket, buffer, 1024, 0)) > 0)
	{
		stroka.append(buffer, indikator);
	}

	Formatirovanie(stroka);
	Formatirovanie2(stroka);

	Info();
	
	cout<< endl;
	cout << stroka << endl;

	cout << endl;
	cout << "Это отформатированный запрос. Данные записанны во внешний файл с названием <<Погода>>" << endl;
	cout << "Соединение завершено" << endl;

	ofstream OutFile("Погода.txt");

	if (!OutFile.is_open())
	{
		cout << "Файл не открыт!" << endl;
	}
	else
	{
		OutFile<< "Погода сегодня: ";
		OutFile<< endl;
		OutFile<< stroka;
		OutFile << endl;
		OutFile << endl;
		OutFile << Razrab();
		OutFile << endl;
		OutFile.close();
	}

	closesocket(firstSocket);
	WSACleanup();

	return 0;
}