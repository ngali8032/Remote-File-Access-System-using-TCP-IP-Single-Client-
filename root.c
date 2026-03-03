#include "header.h"

void takeInput(char *dirAddr, int cfd)
{
	DIR *ptr=NULL;
	int flag=0;
	char buf[100];
	char readbuf[100];

label:
	ptr=opendir(dirAddr);

	strcpy(readbuf, dirAddr);

	//printf("Enter your choice:");
	send(cfd, "\nEnter command:\n", 100, 0);

	recv(cfd, buf, 100, 0);
//	buf[strlen(buf)-1]='\0';

	struct dirent *rd;
	
	if(strcmp(buf+3, "root")==0)
	{
		send(cfd, "permission denied\n", 100, 0);
		goto label;
	}

	if((memcmp(buf, "cd ", 3))==0)
	{
		while((rd=readdir(ptr))!=NULL)
		{
			if((strcmp(buf+3, rd->d_name))==0)
			{
				flag=1;
				struct stat sb;
				strcat(readbuf,rd->d_name);
				stat(readbuf, &sb);

				if(S_ISDIR(sb.st_mode)){
					strcat(dirAddr, buf+3);
					return;
				}
				else{
					send(cfd, "---NOT A DIRECTORY----\n", 100, 0);
					goto label;
				}
			}
		}
		
		if(flag==0)
		{
			send(cfd, "----DIRECTORY DOES NOT EXIST-----\n",100,0);
			goto label;
		}
	}
	else if((memcmp(buf, "open ", 5))==0)
	{	
		while((rd=readdir(ptr))!=NULL)
		{
			if((strcmp(buf+5, rd->d_name))==0)
			{
				flag=1;
				struct stat sb;
				strcat(readbuf,rd->d_name);
				stat(readbuf, &sb);

				if(S_ISREG(sb.st_mode))
				{
					int fd=open(readbuf, O_RDONLY);
					if(fd<0)
					{
						send(cfd, "File failed to open\n", 100, 0);
						perror("open");
						goto label;
					}
					
					/*Extra*/
					printf("files information\n");
					printf("File name:- %s\n", rd->d_name);
					printf("Transferring file content to the client...\n");
					/*Extra*/

					char ch[2];
					while((read(fd, ch, 1))==1){
						ch[1]='\0';
						send(cfd, ch, 100, 0);
						usleep(5000);

					}	
					send(cfd, "File read successfully\n", 100, 0);
					printf("File Transferred successfully\n");/*Extra*/
					send(cfd, "\n", 100, 0);
				}

				else
					send(cfd, "---FILE NOT FOUND---",100,0);
					
				goto label;
			}
		}

		if(flag==0)
		{
			send(cfd,"-- INVALID FILE NAME --",100,0);
			goto label;
		}
	}
	else if(strcmp(buf, "back")==0 && strlen(dirAddr)>2)
	{
		for(int i=strlen(dirAddr)-2; i>=0; i--)
		{
			if(dirAddr[i]!='/')
				dirAddr[i]='\0';
			else if(dirAddr[i]=='/'){
				dirAddr[i]='\0';
				break;
			}
		}
	}
	else if(strcmp(buf, "exit")==0 || strcmp(buf, "quit")==0)
		exit(0);
	else if(strcmp(buf, "path")==0)
	{
		//printf("PATH:------%s\n", dirAddr);
		send(cfd, dirAddr, 100, 0);
		goto label;
	}
	else if(strcmp(buf, "ls")==0 && strlen(dirAddr)>1)
	{
		dirAddr[strlen(dirAddr)-1]='\0';
		return;
	}
	else if(strcmp(buf, "help")==0)
	{
		send(cfd, "\nTO CHANGE DIRECTORY --> cd <directoryName>\n", 100, 0);
        	send(cfd, "TO OPEN FILE --> open <fileName>\n", 100, 0);
        	send(cfd, "TO KNOW PWD --> <path>\n", 100, 0);
        	send(cfd, "TO GO TO PARENT DIRECTORY --> <back>\n", 100, 0);
		send(cfd, "TYPE <exit> or <quit> to quit the connection\n", 100, 0);
		goto label;
	}
	else
	{
		//printf("INVALID INPUT\n");
		send(cfd, "INVALID COMMAND -- type help", 100, 0);
		goto label;
	}
}

void readWriteFilesDir(int cfd)
{
	DIR *dir=NULL;
	char *dirAddr = malloc(100*sizeof(char));
	strcpy(dirAddr, "/");
	
	/*Extra for loading*/
	printf("Clinet Connected\n");
	printf("Loading files");
	fflush(stdout);
	for(int i=0; i<10; i++){
		printf(".");
		usleep(50000);
		fflush(stdout);
	}
	printf("\n");/*Till here*/

	while(1)
	{
		printf("Client Path %s\n", dirAddr);/*For Knowing the clinet path*/
		printf("directories information");
		
		dir = opendir(dirAddr);
		struct dirent *read;

		while((read=readdir(dir))!=NULL){
			if(read->d_name[0]!='.'){
				send(cfd, read->d_name, 100, 0);
				send(cfd, "\n", 100, 0);
				printf(".");
				fflush(stdout);
				usleep(50000);
			}
		}
		printf("\n"); /*Extra*/

			takeInput(dirAddr, cfd);
			strcat(dirAddr, "/");
	}
}
