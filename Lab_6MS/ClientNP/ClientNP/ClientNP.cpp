#include <iostream>
#include <Winsock2.h>
#include <string>
using namespace std;


#define PIPEG TEXT("\\\\DESKTOP-2LAR9DR\\pipe\\Tube")
#define MAX_SIZE_OF_BUFFER 64

string GetErrorMsgText(int code);
string SetPipeError(string msgText, int code);

int main()
{
	try {
		setlocale(LC_ALL, "rus");
		HANDLE cH;

		if ((cH = CreateFile(PIPEG, // Имя канала
			GENERIC_READ | GENERIC_WRITE, // Чтение и запись
			FILE_SHARE_READ | FILE_SHARE_WRITE, // Совместное чтение и запись
			NULL, // аттрибуты безопасности
			OPEN_EXISTING, //  открытие существующего канала
			NULL, // флаги и атрибуты
			NULL)) == INVALID_HANDLE_VALUE) // доп флаги
		{
			throw SetPipeError("CreateFile:", GetLastError());
		}

		DWORD dwWrite;
		char buffer[50] = {"Hello Server"};

		int count;
		cout << "Введите кол-во сообщений: ";
		cin >> count;

		for (int i = 1; i <= count; i++) {
			if (!WriteFile(cH, &buffer, strlen(buffer - 1), &dwWrite, NULL)) {
				throw SetPipeError("WriteFile:", GetLastError());
			}

			if (!ReadFile(cH, buffer, MAX_SIZE_OF_BUFFER, &dwWrite, NULL)) {
				throw SetPipeError("ReadFile:", GetLastError());
			}
			cout << "Сервер прислал СМС: " << buffer << endl;
		}


		if (!CloseHandle(cH)) {
			throw SetPipeError("CloseHandle:", GetLastError());
		}
	}
	catch (string ErrorPipeText) {
		cout << endl << ErrorPipeText;
	}
	
	system("pause");
}

string GetErrorMsgText(int code) // Функция позволяет получить сообщение ошибки
{
	switch (code)                      // check return code   
	{
	case ERROR_ACCESS_DENIED: return "ERROR_ACCESS_DENIED: Отказано в доступе";
	case ERROR_LOGON_FAILURE: return "ERROR_LOGON_FAILURE: 1326: Вход в систему не произведен: имя пользователя или пароль не опознаны.";
	case ERROR_FILE_NOT_FOUND: return "ERROR_FILE_NOT_FOUND: Не удалось найти файл(канал)";
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
	};
}

string SetPipeError(string msgText, int code) // Функция возвращает сообщение ошибки
{
	return msgText + GetErrorMsgText(code);
}

//#pragma region Includes
//#include <stdio.h>
//#include <Windows.h>
//#pragma endregion
//
//#define SERVER_NAME L"Artyom"  // change to your server name!
//#define FULL_SERVER_NAME L"\\\\" SERVER_NAME
//#define PIPE_NAME L"SamplePipe"
//#define FULL_PIPE_NAME FULL_SERVER_NAME L"\\pipe\\" PIPE_NAME
//
//#define BUFFER_SIZE 1024
//
//#define REQUEST_MESSAGE L"Default request from client"
//
//int wmain(int argc, wchar_t* argv[])
//{
//    HANDLE hPipe = INVALID_HANDLE_VALUE;
//    DWORD dwError = ERROR_SUCCESS;
//
//    NETRESOURCE nr;
//    ZeroMemory(&nr, sizeof(nr));
//    nr.dwType = RESOURCETYPE_ANY;
//    nr.lpRemoteName = FULL_SERVER_NAME;
//
//    // These lines make client work with any server (including XP)
//    // even if you not in domain or logged to the server 
//    // But don't forget to ass named pipe name to the registry on server!
//    dwError = WNetAddConnection2(&nr, L"", L"", 0);
//    if (dwError != ERROR_SUCCESS)
//    {
//        wprintf_s(L"WNetAddConnection2 fails");
//        goto Cleanup;
//    }
//
//    while (TRUE)
//    {
//        hPipe = CreateFile(
//            FULL_PIPE_NAME,
//            GENERIC_READ | GENERIC_WRITE,
//            0, // no sharing
//            nullptr, //default security attributes
//            OPEN_EXISTING, //open existing pipe
//            SECURITY_ANONYMOUS,  //default attributes
//            nullptr // no template file
//        );
//
//        if (hPipe != INVALID_HANDLE_VALUE)
//        {
//            wprintf_s(L"The named pipe %s is now connected. \n", FULL_PIPE_NAME);
//            break;
//        }
//
//        dwError = GetLastError();
//
//        if (dwError == ERROR_PIPE_BUSY)
//        {
//            //all pipe instances are busy so wait for 5 secs
//            if (!WaitNamedPipe(FULL_PIPE_NAME, 5000))
//            {
//                dwError = GetLastError();
//                wprintf_s(L"Could not open named pipe. 5 sec timeout expired \n");
//                goto Cleanup;
//            }
//        }
//        else
//        {
//            wprintf_s(L"Unable to open named pipe with error %08lx \n", dwError);
//            goto Cleanup;
//        }
//    }
//
//
//    // Set the read mode and blocking mode of the named pipe. Here , we set data to be read from the pipe as a stream of messages.
//    DWORD dwMode = PIPE_READMODE_MESSAGE;
//    if (!SetNamedPipeHandleState(hPipe, &dwMode, nullptr, nullptr))
//    {
//        dwError = GetLastError();
//        wprintf_s(L"SetNamedPipeHandleState failed with error %08lx \n", dwError);
//        goto Cleanup;
//    }
//
//    //Send request from client to server
//    wchar_t chRequest[] = REQUEST_MESSAGE;
//    DWORD cbRequest, cbWritten;
//
//    cbRequest = sizeof(chRequest);
//
//    if (!WriteFile(hPipe, chRequest, cbRequest, &cbWritten, nullptr))
//    {
//        dwError = GetLastError();
//        wprintf_s(L"WriteFile failed with error message %08lx \n", dwError);
//        goto Cleanup;
//    }
//
//    wprintf_s(L"Sent %ld bytes to server: %s \n", cbWritten, chRequest);
//
//    // Receive a response from the server
//    BOOL fFinishRead = TRUE;
//
//
//    do
//    {
//        wchar_t chResponse[BUFFER_SIZE];
//        DWORD cbResponse, cbRead;
//
//        cbResponse = sizeof(chResponse);
//
//        fFinishRead = ReadFile(hPipe, chResponse, cbResponse, &cbRead, nullptr);
//
//        dwError = GetLastError();
//
//        if (!fFinishRead && ERROR_MORE_DATA != dwError)
//        {
//            wprintf_s(L"Readfile from pipe %s failed with error %08lx \n", FULL_PIPE_NAME, dwError);
//            break;
//        }
//
//        wprintf(L"Receive %ld bytes fom server : %s \n", cbRead, chResponse);
//
//    } while (!fFinishRead);
//
//Cleanup:
//    if (hPipe != INVALID_HANDLE_VALUE)
//    {
//        CloseHandle(hPipe);
//        hPipe = INVALID_HANDLE_VALUE;
//    }
//
//    return dwError;
//}
