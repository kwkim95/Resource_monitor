#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <psapi.h>

double cputime() 
{ 
	FILETIME kernel, user, idle;

	GetSystemTimes(&idle, &kernel, &user);

	DWORD cur_k1 = kernel.dwHighDateTime;
	DWORD prev_k1 = kernel.dwLowDateTime;
	DWORD cur_u1 = user.dwHighDateTime;
	DWORD prev_u1 = user.dwLowDateTime;
	DWORD cur_i1 = idle.dwHighDateTime;
	DWORD prev_i1 = idle.dwLowDateTime;

	Sleep(1000);

	GetSystemTimes(&idle, &kernel, &user);

	DWORD cur_k2 = kernel.dwHighDateTime;
	DWORD prev_k2 = kernel.dwLowDateTime;
	DWORD cur_u2 = user.dwHighDateTime;
	DWORD prev_u2 = user.dwLowDateTime;
	DWORD cur_i2 = idle.dwHighDateTime;
	DWORD prev_i2 = idle.dwLowDateTime;

	double k = (cur_k2 + prev_k2) - (cur_k1 + prev_k1); // Ŀ�� �̿��
	double u = (cur_u2 + prev_u2) - (cur_u1 + prev_u1); // user �̿��
	double i = (cur_i2 + prev_i2) - (cur_i1 + prev_i1); // idle
	
	double cpu = (k + u - i) * 100 / (k + u); // CPU ���� ���ϴ� ���� = (kernel+user-idle) * 100 / (kernel+user)

	return cpu;
}

int main() 
{
	int count = 0;
	SYSTEMTIME lt; // Current Time�� �����ϴ� ����
	SYSTEM_INFO Info; // Number of processors�� �����ϴ� ����
	GetSystemInfo(&Info);

	DWORD Process[1024], CountBytes;
	int NumOfProc; // number of processes�� �����ϴ� ����
	EnumProcesses(Process, sizeof(Process), &CountBytes); 
	NumOfProc = CountBytes / sizeof(DWORD);

	MEMORYSTATUSEX Memory; // Current Memory Load�� �����ϴ� ����
	Memory.dwLength = sizeof(Memory);
	GlobalMemoryStatusEx(&Memory);

	printf("Number of CPU's : %d\n", Info.dwNumberOfProcessors); 
	printf("Index\t\tTime\t\tProcesses\tMemoryLoad\tCPULoad\n");

	while (1) 
	{
		count++;
		GetLocalTime(&lt);
		double r = cputime();

		printf("%3d\t ", count);
		printf("%02d_%02d_%02d %02d_%02d_%02d : ", lt.wYear, lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond); // current time
		printf("\t   %d", NumOfProc); // number of processes
		printf("\t\t   %d%c", Memory.dwMemoryLoad, 37); // current memory load

		if (count <= 5) // 5�� ���� cpu load
			printf("\t\t%0.2f\n", r);
		else if (count > 5 && count <= 10) // 6���̻� 10������ cpu load
		{ 
			printf("\t\t%0.2f\t[5sec avg : %0.2f]\n", r, (double) (r-4 + r - 3 + r - 2 + r - r) / 5);
		}
		else if (count > 10 && count <= 15) // 11���̻� 15������ cpu load
		{
			printf("\t\t%0.2f\t[5sec avg : %0.2f] [10sec avg : %0.2f]\n",r, (double)(r - 4 + r - 3 + r - 2 + r - r) / 5,
				(double)(r - 9 + r - 8 + r - 7 + r - 6 + r -5 + r - 4 + r - 3 + r - 2 + r - r) / 10);
		}
		else if (count > 15) // 16���̻� cpu load
		{
			printf("\t\t%0.2f\t[5sec avg : %0.2f] [10sec avg : %0.2f] [15sec avg : %0.2f]\n", r, (double)(r - 4 + r - 3 + r - 2 + r - r) / 5,
				(double)(r - 9 + r - 8 + r - 7 + r - 6 + r - 5 + r - 4 + r - 3 + r - 2 + r - r) / 10, 
				(double)(r - 14 + r - 13 + r - 12 + r - 11 + r - 10 + r - 9 + r - 8 + r - 7 + r - 6 + r - 5 + r - 4 + r - 3 + r - 2 + r - r) / 15);
		}
	}
}

