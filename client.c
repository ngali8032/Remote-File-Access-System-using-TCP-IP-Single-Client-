#include "header.h"

int main(int argc, char *argv[])
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in addr;
	int len = sizeof(addr);
		
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = inet_addr(argv[2]);
	
	connect(fd, (struct sockaddr*)&addr, len);
	
	char buf[100];
	int status;

	if(fork()){
		while(1){
			fgets(buf, 100, stdin);
			buf[strlen(buf)-1] = '\0';
	
			status=send(fd, buf, 100, 0);
			
			if(status==0)
				break;
			else if(status<0)
				break;
		}
	}
	else
	{
		while(1){
			status=recv(fd, buf, 100, 0);
			if(status<0)
			{
				perror("recv");
				break;
			}
			else if(status==0)
			{
				printf("server terminated without 3-way handshaking\n");
				break;
			}
			else{
				printf("%s", buf);
				if(strcmp(buf, "quit")==0)
					break;
			}
		}
	}
	printf("Client side done\n");
	close(fd);
}
