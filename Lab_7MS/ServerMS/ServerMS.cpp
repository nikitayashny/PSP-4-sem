#include <iostream>
#include "windows.h"
#include <string>

using namespace std;

string GetErrorMail(int code)
{
	string msgText = "";
	switch (code)
	{
	case WSAEINTR: msgText = "WSAEINTR"; break;
	case WSAEACCES:	msgText = "WSAEACCES"; break;
	case WSAEFAULT:	msgText = "WSAEFAULT"; break;
	default: msgText = "Error";
	};
	return msgText;
}

string SetErrorMail(string msgText, int code)
{
	return msgText + GetErrorMail(code);
}

int main()
{
	setlocale(LC_ALL, "rus");

	try {
		HANDLE serverMailSlot;
		int i = 0;
		double t1, t2;
		
		SECURITY_DESCRIPTOR  pSD;
		SECURITY_ATTRIBUTES  SA;

		InitializeSecurityDescriptor(&pSD, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&pSD, true, NULL, false);
		SA.nLength = sizeof(SA);
		SA.lpSecurityDescriptor = &pSD;
		SA.bInheritHandle = true;

		
		//возвращает дескриптор почтового ящика
		if ((serverMailSlot = CreateMailslot(L"\\\\.\\mailslot\\BOX", // символическое имя ящика
			/*300 ,*/500,							//максимальную длину сообщения равную 300 байт
			MAILSLOT_WAIT_FOREVER, /*180000,*/	//бесконечный интервал ожидания для функции ReadFile
			&SA)) == INVALID_HANDLE_VALUE)			//атрибуты безопасности
			throw SetErrorMail("CreateMailslot", GetLastError());

		cout << "Hello i am server" << endl;

		char readBuf[50];
		DWORD readMsg;
		do {
			i++;
			if (!ReadFile(serverMailSlot, readBuf, sizeof(readBuf), &readMsg, NULL))
				throw SetErrorMail("ReadFile", GetLastError());
			SetMailslotInfo(serverMailSlot, 5000);
			if (i == 1)
				t1 = clock();
			cout << readBuf << " " << i << endl;
			//Sleep(10);
		} while (strcmp(readBuf, " ") != 0);

		t2 = clock();

		cout << "Время передачи: " << (t2 - t1) / 1000 << " сек." << endl;
		cout << "Количество сообщений: " << i - 1 << endl << endl;

		if (!CloseHandle(serverMailSlot))
			throw "Error: CloseHandle";

		system("pause");
	}
	catch (string e) {
		if (e == "ReadFileError") {
			cout << e << " Timeout";
			return 0;
		}
		cout << e << endl;
	}
	return 0;
}