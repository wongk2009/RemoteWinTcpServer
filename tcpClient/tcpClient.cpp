// tcpClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <WinSock2.h> 

#define PORT 8087 
#define SERVER_IP "127.0.0.1" 
#define BUFFER_SIZE 1024 
#define FILE_NAME_MAX_SIZE 512 
#pragma comment(lib, "WS2_32") 

int main()
{
	// 初始化socket dll 
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 0);
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		printf("Init socket dll error!");
		exit(1);
	}

	//创建socket 
	SOCKET c_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == c_Socket)
	{
		printf("Create Socket Error!");
		system("pause");
		exit(1);
	}

	//指定服务端的地址 
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	server_addr.sin_port = htons(PORT);

	if (SOCKET_ERROR == connect(c_Socket, (LPSOCKADDR)& server_addr, sizeof(server_addr)))
	{
		printf("Can Not Connect To Server IP!\n");
		system("pause");
		exit(1);
	}

	//输入文件名 
	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	printf("Please Input File Name On Server: ");
	scanf("%s", &file_name);

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));

	//向服务器发送文件名 
	if (send(c_Socket, buffer, BUFFER_SIZE, 0) < 0)
	{
		printf("Send File Name Failed\n");
		system("pause");
		exit(1);
	}

	FILE* fp = fopen(file_name, "rb"); //windows下是"rb",表示打开一个只读的二进制文件 
	if (NULL == fp)
	{
		printf("File: %s Not Found\n", file_name);
	}
	else
	{
		memset(buffer, 0, BUFFER_SIZE);
		int length = 0;

		while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
		{
			if (send(c_Socket, buffer, length, 0) < 0)
			{
				printf("Send File: %s Failed\n", file_name);
				break;
			}
			memset(buffer, 0, BUFFER_SIZE);
		}

		fclose(fp);
		printf("File: %s Transfer Successful!\n", file_name);
	}

	//打开文件，准备写入 
	//FILE* fp = fopen(file_name, "wb"); //windows下是"wb",表示打开一个只写的二进制文件 
	//if (NULL == fp)
	//{
	//	printf("File: %s Can Not Open To Write\n", file_name);
	//	system("pause");
	//	exit(1);
	//}
	//else
	//{
	//	memset(buffer, 0, BUFFER_SIZE);
	//	int length = 0;
	//	while ((length = recv(c_Socket, buffer, BUFFER_SIZE, 0)) > 0)
	//	{
	//		if (fwrite(buffer, sizeof(char), length, fp) < length)
	//		{
	//			printf("File: %s Write Failed\n", file_name);
	//			break;
	//		}
	//		memset(buffer, 0, BUFFER_SIZE);
	//	}

	//	printf("Receive File: %s From Server Successful!\n", file_name);
	//}

	//fclose(fp);
	closesocket(c_Socket);

	//释放winsock库 
	WSACleanup();

	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
