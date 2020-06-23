#include<stdio.h>
#include<WinSock2.h>

// ����� ID �� PW ���� ��ġ ����
#define USERFILE "C:\\Users\\Admin\\userid.txt"
#define _CRT_SECURE_NO_WARNINGS
//ID �� PWD ȣ�� �Լ� getid ����
void getid(void);

//TCP/IP ��Ű� ���Ǵ� ��ټ��� �Լ����� <WinSock2.h> ����� �� ����Ǿ� �ִ�. ��͸� ������ ����!
#pragma comment(lib, "wsock32.lib")

// ������ IDLIST �� ����ü tagIDList ����
typedef struct tagIDList
{
	char uid[9];
	char pwd[9];
}IDLIST;
//IDLIST Ÿ�� ����ü 100�� �迭�� id ������ ����
IDLIST id[100];
//<WinSock2.h> �Լ����� �Ϲ����� C��� ��Ÿ�� ���̺귯���� ���ԵǾ� ���� �ʱ� ������, �ش� ���̺귯�� ������ �ʿ��ϴ�.
// <WinSock2.h> ��� ���Խ� ���� �����Ͽ� ����!
/* ���� ���� ���α׷��� ��ȯ ������
	WSADATA ���� ���� ������ ��ȯ �� ����ü ����(sin, cli_addr)
	WSAStartup ���� ���� ����
	Socket s; ���� ����
	bind ���� ��Ʈ �� ip ����
	accept ���� ����
	closesocket(s) ���� ���
	WSACleanup ���� �ڿ� ����
	������ �̾�����.
*/
void main(void)
{
	//������ ����ϱ� ���� ��ũ���� ����, ���� ��ȣ��� �� �� ������, File* fd(file descriptor) �� ���
	SOCKET s, cs;
	//WSA ���� Ȯ���ϴ� �Լ�, �ҷ��� wsaData �� struct �̴�.
	WSADATA wsaData;

	//���� ����ü sin, cli_addr ����
	struct sockaddr_in sin;
	struct sockaddr_in cli_addr;
	int size = sizeof(cli_addr);
	char data[1000];
	int ret, i;

	//����ü login ������ ����
	IDLIST login;
	//�ش� ����ü ���� getid �Լ� ����
	getid();

	//���� ��밡������ �ü�� �����Ϳ� ���ϴ� �Լ�, WSAStartup(a,b) �Լ��� a == b �̸� 0�� ��ȯ�Ѵ�
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		printf("WSAStartup ����, �����ڵ� = %d\n", WSAGetLastError());
		return;
	}


	//TCP/IP ���� ����, ������ ��� ����bind �Լ�
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// ���� ������ �ȵǾ��� ��� �����ڵ� ���, ��κ� WSAStartup �Լ� �Ⱦ��ų�, �߸��� �μ� ����� ��� �߻�!
	if (s == INVALID_SOCKET)
	{
		printf("���� ���� ����, �����ڵ� : %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}


	//�ּ� ü�� ���� : IPv4 (���� �������ݴ��� ��� AF_UNIX, IPv6 �� ��� AF_INET6)
	sin.sin_family = AF_INET;
	//���� ��� : ��� Ŭ���̾�Ʈ ���
	sin.sin_addr.s_addr = htonl(ADDR_ANY);
	//��Ʈ ��ȣ ���� : 10000(1 ~ 65535 ���� ��� ����, �� 1 ~ 2000�� ���̴� �ü�� �� ��� �Ҵ��. 21�� FTP, 80�� �� ���� ��Ʈ)
	sin.sin_port = htons(10000);

	//IP �� ��Ʈ ���ε� �� ���� �� ���� ����
	if (bind(s, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf(" ���ε� ����, ���� �ڵ� : %u\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return;
	}

	// ���� ��� ���� ���� �� ���� ����
	if (listen(s, SOMAXCONN) != 0)
	{
		printf(" ���� ��� ���� ����, ���� �ڵ� : %u\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return;
	}
	puts("���� ���� ����");





	while (1)
	{
		cs = accept(s, (struct sockaddr*)&cli_addr, &size);
		if (cs == INVALID_SOCKET)
		{
			printf(" ���� ���� ����, ���� �ڵ� : %u\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return;
		}

		// data ���� 0���� �ʱ�ȭ
		memset(data, 0, sizeof data);

		ret = recv(cs, data, 1000, 0);
		if (ret == SOCKET_ERROR)
		{
			printf("���� ����, ���� �ڵ� : %u\n", WSAGetLastError());
			closesocket(cs);
			closesocket(s);
			WSACleanup();
			return;
		}
		//login ������ 0���� �ʱ�ȭ
		memset(&login, 0, sizeof(login));
		//data ������ login ����
		strncpy(login.uid, &data[0], 8);
		strncpy(login.pwd, &data[8], 8);

		//���� ����
		for (i = 7; i >= 0; i--)
		{
			if (isspace(login.uid[i]))
				login.uid[i] = 0;
			if (isspace(login.pwd[i]))
				login.pwd[i] = 0;
		}
		//���� ��ü Ȯ���Ͽ� �� �ǽ�
		for (i = 0; i < 100; i++)
		{
			if (strcmp(login.uid, id[i].uid) == 0)
			{
				if (strcmp(login.pwd, id[i].pwd) == 0)
				{
					printf("%s ����� ����\n", login.uid);
					send(cs, "1", 1, 0);
					break;
				}
				else
				{
					printf("%s ����� ��ȣ�� ��Ȯ���� �ʽ��ϴ�.\n", login.uid);
					send(cs, "2", 1, 0);
					break;
				}
			}
		}
		// ���� ��ü ��� ���� �ش���� ���� ��� if �� ���
		if (i == 100)
		{
			printf("��ϵ��� ���� ����ڰ� �α����� �õ��Ͽ����ϴ�\n");
			printf("����� ID : %s\n", login.uid);
			send(cs, "3", 1, 0);
		}
		closesocket(cs);

	}

	//���� ����. ���� �����־ ��� ����, ������ ��� ����
	if (closesocket(cs) != 0 || closesocket(s) != 0)
	{
		printf("���� ���� ����, �����ڵ� = %u\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	// ���� ���� ����� �ڿ� ��ü ���� �ǽ�
	if (WSACleanup() != 0)
	{
		printf("WSACleanup ����, �����ڵ� = %u\n", WSAGetLastError());
		return;
	}
}
//struct tagIDlist ����, fopen ���� userid ���� ����� ���� �Լ�
void getid(void)
{
	FILE *fp;
	int i, j;
	char buff[1000];
	fp = fopen(USERFILE, "r");

	for (i = 0; i < 100; i++)
	{
		fgets(buff, 20, fp);
		strncpy(id[i].uid, &buff[0], 8);
		strncpy(id[i].pwd, &buff[8], 8);

		for (j = 7; j >= 7; j--)
		{
			if (isspace(id[i].uid[j]))
				id[i].uid[j] = 0;
			if (isspace(id[i].pwd[j]))
				id[i].pwd[j] = 0;
		}
	}
	fclose(fp);
}
