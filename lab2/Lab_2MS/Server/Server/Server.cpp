#include <iostream>
#include "Winsock2.h"			   // Заголовок WS2_32.dll

#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#pragma warning(suppress : 4996)							
#pragma warning(disable : 4996)

using namespace std;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);


int main()
{
	setlocale(LC_ALL, "rus");
	int i;
	int t;
	int t2;
	try {
		WSADATA wsaData;	//Версия WS
		SOCKET serverSocket; // серверный сокет

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)                   // инициализация библиотеки
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
												//ориентирован на передачу потоком
		if ((serverSocket = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) // создать сокет
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;				   // параметры сокета sS
		serv.sin_family = AF_INET;		   // используется IP-адресация
		serv.sin_port = htons(2000);	   // порт 2000 (Для преобразования номера порта в формат TCP/IP)
		serv.sin_addr.s_addr = INADDR_ANY; // любой собственный IP - адрес

		if (bind(serverSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) // установка параметров serv для сокета serverSocket
			/*Функция связывает дескриптор сокета и структуру SOCKADDR_IN,
			которая предназначена для хранения параметров сокета*/

			throw  SetErrorMsgText("bind:", WSAGetLastError());

		if (SOCKET_ERROR == listen(serverSocket, 2)) // установка сокета сервера в режим прослушивания (2 параметр - макс. длина очереди)
		{
			cout << "Listen: " << WSAGetLastError << endl; 
		}
		cout << "Сервер начал прослушку!" << endl;

		SOCKET clientSocket;	                 // сокет для обмена данными с клиентом 
		SOCKADDR_IN clnt;						 // параметры  сокета клиента
		memset(&clnt, 0, sizeof(clnt));			 // обнулить память
		int lclnt = sizeof(clnt);			     // размер SOCKADDR_IN



		while (true) {
			i = 1;
			// Функция accept создает канал на стороне сервера и иоздает сокет для обмена данными по этому каналу
			if ((clientSocket = accept(serverSocket, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
				throw  SetErrorMsgText("accept:", WSAGetLastError());

			// Начал замер!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			t = clock();

			cout << "Соединение с клиентом успешно!" << endl;
			cout << "Адрес клиента: " << inet_ntoa(clnt.sin_addr) << ":" << htons(clnt.sin_port) << "\n\n\n\n\n";

			char ibuf[50],                     //буфер ввода 
				obuf[50] = "sever: принято ";  //буфер вывода
			int  libuf = 0,                    //количество принятых байт
				lobuf = 0;

			while (true) {
				
				

				//остановка до вызова connect, результат - сокет для связи
				if ((libuf = recv(clientSocket, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
				{
					cout << "Recv: " << WSAGetLastError() << endl;
					break;
				}


				_itoa(lobuf, obuf + sizeof("sever: принято ") - 1, 10);

				cout << "Принято сообщение " << i << ": " << ibuf << endl;
				i++;

				if ((lobuf = send(clientSocket, ibuf, strlen(ibuf) + 1, NULL)) == SOCKET_ERROR)
				{
					cout << "Send: " << WSAGetLastError() << endl;
					break;
				}



			}

			t2 = clock();

			cout << "\n\nКлиент отключился" << endl;
			cout << "Затраченное время: " << (((float)t2) - t) /*/ CLOCKS_PER_SEC*/ << " миллисекунд";
		}
		if (closesocket(serverSocket) == SOCKET_ERROR)							// закрыть сокет
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)							    // завершить работу с библиотекой
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());

		
	}
	catch (string errorMsgText) {
		{ cout << endl << "WSAGetLastError: " << errorMsgText; }
	}
	return 0;
}

string GetErrorMsgText(int code) // Функция позволяет получить сообщение ошибки
{
    string msgText;
    switch (code)
    {
	case WSAEINTR: return "WSAEINTR: Работа функции прервана ";
	case WSAEACCES: return "WSAEACCES: Разрешение отвергнуто";
	case WSAEFAULT:	return "WSAEFAULT: Ошибочный адрес";
	case WSAEINVAL:	return "WSAEINVAL: Ошибка в аргументе";
	case WSAEMFILE: return "WSAEMFILE: Слишком много файлов открыто";
	case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK: Ресурс временно недоступен";
	case WSAEINPROGRESS: return "WSAEINPROGRESS: Операция в процессе развития";
	case WSAEALREADY: return "WSAEALREADY: Операция уже выполняется";
	case WSAENOTSOCK: return "WSAENOTSOCK: Сокет задан неправильно";
	case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ: Требуется адрес расположения";
	case WSAEMSGSIZE: return "WSAEMSGSIZE: Сообщение слишком длинное";
	case WSAEPROTOTYPE: return "WSAEPROTOTYPE: Неправильный тип протокола для сокета";
	case WSAENOPROTOOPT: return "WSAENOPROTOOPT: Ошибка в опции протокола";
	case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT: Протокол не поддерживается";
	case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT: Тип сокета не поддерживается";
	case WSAEOPNOTSUPP:	return "WSAEOPNOTSUPP: Операция не поддерживается";
	case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT: Тип протоколов не поддерживается";
	case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT: Тип адресов не поддерживается протоколом";
	case WSAEADDRINUSE:	return "WSAEADDRINUSE: Адрес уже используется";
	case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL: Запрошенный адрес не может быть использован";
	case WSAENETDOWN: return "WSAENETDOWN: Сеть отключена";
	case WSAENETUNREACH: return "WSAENETUNREACH: Сеть не достижима";
	case WSAENETRESET: return "WSAENETRESET: Сеть разорвала соединение";
	case WSAECONNABORTED: return "WSAECONNABORTED: Программный отказ связи";
	case WSAECONNRESET:	return "WSAECONNRESET: Связь восстановлена";
	case WSAENOBUFS: return "WSAENOBUFS: Не хватает памяти для буферов";
	case WSAEISCONN: return "WSAEISCONN: Сокет уже подключен";
	case WSAENOTCONN: return "WSAENOTCONN: Сокет не подключен";
	case WSAESHUTDOWN: return "WSAESHUTDOWN: Нельзя выполнить send : сокет завершил работу";
	case WSAETIMEDOUT: return "WSAETIMEDOUT: Закончился отведенный интервал  времени";
	case WSAECONNREFUSED: return "WSAECONNREFUSED: Соединение отклонено";
	case WSAEHOSTDOWN: return "WSAEHOSTDOWN: Хост в неработоспособном состоянии";
	case WSAEHOSTUNREACH: return "WSAEHOSTUNREACH: Нет маршрута для хоста";
	case WSAEPROCLIM: return "WSAEPROCLIM: Слишком много процессов";
	case WSASYSNOTREADY: return "WSASYSNOTREADY: Сеть не доступна";
	case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED: Данная версия недоступна";
	case WSANOTINITIALISED:	return "WSANOTINITIALISED: Не выполнена инициализация WS2_32.DLL";
	case WSAEDISCON: return "WSAEDISCON: Выполняется отключение";
	case WSATYPE_NOT_FOUND: return "WSATYPE_NOT_FOUND: Класс не найден";
	case WSAHOST_NOT_FOUND:	return "WSAHOST_NOT_FOUND: Хост не найден";
	case WSATRY_AGAIN: return "WSATRY_AGAIN: Неавторизированный хост не найден";
	case WSANO_RECOVERY: return "WSANO_RECOVERY: Неопределенная ошибка";
	case WSANO_DATA: return "WSANO_DATA: Нет записи запрошенного типа";
	case WSA_INVALID_HANDLE: return "WSA_INVALID_HANDLE: Указанный дескриптор события с ошибкой";
	case WSA_INVALID_PARAMETER: return "WSA_INVALID_PARAMETER: Один или более параметров с ошибкой";
	case WSA_IO_INCOMPLETE:	return "WSA_IO_INCOMPLETE: Объект ввода - вывода не в сигнальном состоянии";
	case WSA_IO_PENDING: return "WSA_IO_PENDING: Операция завершится позже";
	case WSA_NOT_ENOUGH_MEMORY: return "WSA_NOT_ENOUGH_MEMORY: Не достаточно памяти";
	case WSA_OPERATION_ABORTED: return "WSA_OPERATION_ABORTED: Операция отвергнута";
	case WSASYSCALLFAILURE: return "WSASYSCALLFAILURE: Аварийное завершение системного вызова";
	default: return "**ERROR**";
	}
	//TODO: добавить коды
	//return msgText;
}


string SetErrorMsgText(string msgText, int code) // Функция возвращает сообщение ошибки
{
	return msgText + GetErrorMsgText(code);
}