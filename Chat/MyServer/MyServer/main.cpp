#pragma comment(lib,"Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>

SOCKET Connect;
SOCKET* Connections;
SOCKET Listen;

int ClientCount = 0;

using namespace std;

void SendMessageToClient(int ID)
{
	char* buffer = new char[1024];
	for (;; Sleep(75))
	{
		memset(buffer, 0, sizeof(buffer)); //очищаем буфер
		//получаем сообщение от сервера
		if (recv(Connections[ID], buffer, 1024, NULL))
		{
			cout << buffer << endl;	// выводим данное сообщение
			//рассылаем это сообщением всем пользовател€м
			for (int i = 0; i <= ClientCount; i++)
			{
				send(Connections[i], buffer, strlen(buffer), NULL);
			}
		}
	}
	delete buffer; //удал€ем буфер
}

int main()
{
	setlocale(LC_ALL, "Russian");
	WSAData data;
	WORD version = MAKEWORD(2, 2); //задаЄм версию
	int res = WSAStartup(version, &data);
	if (res != 0)
	{
		return 0;
	}

	struct addrinfo hints;
	struct addrinfo * result;


	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));

	ZeroMemory(&hints, sizeof(hints));
	//настраиваем сокет
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// приводим сервер в рабочее состо€ние
	getaddrinfo(NULL, "7770", &hints, &result);
	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);
	listen(Listen, SOMAXCONN);	//задаЄм максимальное количество клиентов

	freeaddrinfo(result);

	cout << "Server connect..." << endl;

	char m_connetc[] = "Connect...;;;5";
	for (;; Sleep(75))
	{
		//ќжидаем подключение клиента
		if (Connect = accept(Listen, NULL, NULL))
		{
			cout << "Client connect..." << endl;
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connetc, strlen(m_connetc), NULL);
			ClientCount++;
			CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount - 1), NULL, NULL);
		}

	}

	return 1;
}