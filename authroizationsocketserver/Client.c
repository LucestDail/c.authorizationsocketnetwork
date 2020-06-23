
[Client code]
#include<stdio.h>
#include<WinSock2.h>
//TCP/IP ��Ű� ���Ǵ� ��ټ��� �Լ����� <WinSock2.h> ����� �� ����Ǿ� �ִ�. ��͸� ������ ����!
#pragma comment(lib, "wsock32.lib")
//<WinSock2.h> �Լ����� �Ϲ����� C��� ��Ÿ�� ���̺귯���� ���ԵǾ� ���� �ʱ� ������, �ش� ���̺귯�� ������ �ʿ��ϴ�.
// <WinSock2.h> ��� ���Խ� ���� �����Ͽ� ����!
/* ũ�� ���ư��� ������
	WSADATA ���� ���� ������ ��ȯ
	WSAStartup ���� ���� ����
	Socket s; ���� ����
	closesocket(s) ���� ���
	WSACleanup ���� �ڿ� ����
	������ �̾�����.
*/

void main(int argc, char *argv[])
{
	SOCKET s;
	//WSA ���� Ȯ���ϴ� �Լ�, �ҷ��� wsaData �� struct �̴�.
	WSADATA wsaData;
	//���� ����ü sin ���Ƿ� ����
	struct sockaddr_in sin;
	char data[1000];
	char ID[100], PWD[100];

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
	puts("���� ������ �õ��մϴ�...");
	//���� ���� ���� �Է�(INET �� IPv4, ����ȣ��Ʈ ���� �� ��Ʈ 10000 ����
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(10000);
	if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) != 0)
	{
		printf(" ���� ����, ���� �ڵ� : %u\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return;
	}

	puts("���� ���� �Ϸ�");
	puts("���� ���� ����");
	printf("[ID �Է�] : ");
	gets(ID);
	printf("[PWD �Է�] : ");
	gets(PWD);
	sprintf(data, "%-8.8s%-8.8s", ID, PWD);
	//���� �Լ�
	if (send(s, data, strlen(data), 0) < strlen(data))
	{
		printf("���� ����, ���� �ڵ� = %u\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return;
	}
	//���� �Լ�
	if (recv(s, data, 1, 0) < 1)
	{
		printf("���� ����, ���� �ڵ� = %u\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return;
	}

	if (data[0] == '1')
	{
		puts("�α��� ����");
	}
	else if (data[0] == '2')
	{
		puts("�α��� ����");
	}
	else
	{
		puts("ID ����");
	}
	//���� ����
	if (closesocket(s) != 0)
	{
		printf("���� ���� ����, �����ڵ� = %u\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	//���� ����
	if (WSACleanup() != 0)
	{
		printf("WSACleanup ����, �����ڵ� = %u\n", WSAGetLastError());
		return;
	}
}
