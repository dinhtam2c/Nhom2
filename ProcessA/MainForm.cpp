#include "MainForm.h"

#define SIZE 4096

using namespace System;
using namespace System::Windows::Forms;
using namespace ProcessA;

HANDLE hFileMapping;
PVOID pBuffer;
HANDLE S1;
HANDLE S2;

[STAThread]
void main(array<String^>^ args)
{
	S1 = CreateSemaphoreA(NULL, 0, 1, "N2S1");
	S2 = CreateSemaphoreA(NULL, 0, 1, "N2S2");
	if (!S1 || !S2)
	{
		MessageBox::Show("CreateSemaphore failed: " + GetLastError());
		return;
	}
	hFileMapping = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SIZE, "N2FM");
	if (!hFileMapping)
	{
		MessageBox::Show("CreateFileMapping failed" + GetLastError());
		return;
	}
	pBuffer = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!pBuffer)
	{
		MessageBox::Show("MapViewOfFile failed" + GetLastError());
		return;
	}

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MainForm);
}

void SendMsg(PVOID buffer, DWORD size)
{
	memcpy(pBuffer, buffer, size);
	ReleaseSemaphore(S1, 1, NULL);
	WaitForSingleObject(S2, INFINITE);
}

void SendFile(LPCWSTR filePath)
{
	HANDLE hFile = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox::Show("CreateFile failed: " + GetLastError());
		return;
	}
	DWORD fileSizeHigh = 0;
	DWORD fileSizeLow = GetFileSize(hFile, &fileSizeHigh);
	if (fileSizeLow == INVALID_FILE_SIZE)
	{
		CloseHandle(hFile);
		MessageBox::Show("GetFileSize failed: " + GetLastError());
		return;
	}
	ULONG64 fileSize = ((ULONG64)fileSizeHigh << 32) | fileSizeLow;
	SendMsg((PVOID)filePath, (wcslen(filePath) + 1) * sizeof(wchar_t));
	SendMsg(&fileSize, sizeof(fileSize));
	do
	{
		DWORD bytesRead;
		if (!ReadFile(hFile, pBuffer, min(fileSize, SIZE), &bytesRead, NULL))
		{
			CloseHandle(hFile);
			MessageBox::Show("ReadFile failed: " + GetLastError());
			return;
		}
		fileSize -= bytesRead;
		ReleaseSemaphore(S1, 1, NULL);
		WaitForSingleObject(S2, INFINITE);
	} while (fileSize > 0);
	CloseHandle(hFile);
	MessageBox::Show("File sent successfully");
}