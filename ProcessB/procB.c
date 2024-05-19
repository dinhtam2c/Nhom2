#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <Windows.h>

#define SIZE 4096

HANDLE hFileMapping;
PVOID pBuffer;
HANDLE S1;
HANDLE S2;

void ReadMsg(PVOID buffer, DWORD size)
{
	WaitForSingleObject(S1, INFINITE);
	memcpy(buffer, pBuffer, size);
	ReleaseSemaphore(S2, 1, NULL);
}

int main()
{
	S1 = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "N2S1");
	S2 = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, "N2S2");
	if (!S1 || !S2)
	{
		printf("OpenSemaphore failed: %d\n", GetLastError());
		system("pause");
		return 1;
	}
	hFileMapping = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "N2FM");
	if (!hFileMapping)
	{
		printf("OpenFileMapping failed: %d\n", GetLastError());
		system("pause");
		return 1;
	}
	pBuffer = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (!pBuffer)
	{
		printf("MapViewOfFile failed: %d\n", GetLastError());
		system("pause");
		return 1;
	}

	_setmode(_fileno(stdout), _O_U16TEXT);

	while (1)
	{
		WCHAR filePath[MAX_PATH];
		ReadMsg(filePath, sizeof(filePath));
		PWCHAR fileName = filePath + wcslen(filePath);
		while (fileName > filePath && *fileName != L'\\')
			fileName--;
		fileName++;
		wprintf(L"Receiving file: %s\n", fileName);
		HANDLE hFile = CreateFileW(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		ULONG64 fileSize;
		ReadMsg(&fileSize, sizeof(fileSize));
		wprintf(L"File size: %llu bytes\n", fileSize);
		do
		{
			WaitForSingleObject(S1, INFINITE);
			DWORD bytesWritten;
			WriteFile(hFile, pBuffer, min(fileSize, SIZE), &bytesWritten, NULL);
			fileSize -= bytesWritten;
			ReleaseSemaphore(S2, 1, NULL);
		} while (fileSize > 0);
		CloseHandle(hFile);
		wprintf(L"File received\n\n");
	}

	return 0;
}