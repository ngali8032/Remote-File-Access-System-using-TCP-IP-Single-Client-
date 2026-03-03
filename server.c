#include "header.h"

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		puts("exe portNumber");
		return 0;
	}

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd<0)
	{
		perror("socket");	
		return 0;
	}
	printf("Socket successfully created\n");
	
	struct sockaddr_in sAddr;
	sAddr.sin_family=AF_INET;
	sAddr.sin_port=htons(atoi(argv[1]));
	sAddr.sin_addr.s_addr=INADDR_ANY;
	int slen = sizeof(sAddr);
	if(bind(sfd, (const struct sockaddr*)&sAddr, slen)<0)
	{
		perror("bind");
		return 0;
	}
	printf("Binded successfully\n");	

	if(listen(sfd, 1)<0)
	{
		perror("listen");
		return 0;
	}
	printf("Listening for clients\n");	

	struct sockaddr_in cAddr;
	int clen = sizeof(cAddr);
	
	int cfd=accept(sfd, (struct sockaddr*)&cAddr, &clen);
	if(cfd<0)
	{
		perror("accept");
		return 0;
	}
	
	char *IP = inet_ntoa(cAddr.sin_addr);
	unsigned short port=ntohs(cAddr.sin_port);
	
	printf("clientIp=%s\n", IP);
	printf("ClientPort=%d\n", port);
	char buf[100];
	int status;
	
	char ch[100];
	
	/*send(cfd, "\n---------WELCOME TO COMMAND WINDOW----------\n", 100, 0);
	send(cfd, "\nTO CHANGE DIRECTORY --> cd <directoryName>\n", 100, 0);
	send(cfd, "TO OPEN FILE --> open <fileName>\n", 100, 0);
	send(cfd, "TO KNOW PWD --> <path>\n", 100, 0);
	send(cfd, "TO GO TO PARENT DIRECTORY --> <back>\n", 100, 0);*/

	send(cfd, "\nPlease type the command....\n",100, 0);
	
	while(1){
		recv(cfd, ch, 100, 0);
	
		if(strcmp(ch, "cd/")==0){
			send(cfd, "---ROOT DIRECTORIE'S---\n\n", 100, 0);
			readWriteFilesDir(cfd);
		}
		else{
			printf("client msg:%s\n", ch);
			send(cfd, "Invalid Command\n", 100, 0);
			send(cfd, "To start the process type cd/ command\n", 100, 0);

		}
	}
}
