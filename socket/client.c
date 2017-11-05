#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <netdb.h>

int main(int argc, char **argv)
{
	int clientSoc = 0;
	struct sockaddr_in clientaddrin;
	char msg[100] = "AATGC";
	unsigned char ch = '\0';
	char hostName[255] = "";
	struct hostent *hostp = NULL;

	clientSoc = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&clientaddrin,sizeof(clientaddrin));
	clientaddrin.sin_family = AF_INET;
	clientaddrin.sin_port = htons(9999);
	clientaddrin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (argc > 1)
	{
		strcpy(hostName,argv[1]);
		printf("server HOST/ IP %s, port = 9999\n",hostName);
		hostp = gethostbyname(hostName);
		 if(hostp == (struct hostent *)NULL)
		 {
			 printf("HOST NOT FOUND --> ");
			 /* h_errno is usually defined */
			 /* in netdb.h */
			 perror("h_errno = ");
			 printf("---This is a client program---\n");
			 printf("Command usage: %s <server name or IP>\n", argv[0]);
			 close(clientSoc);
		 }

		 memcpy(&clientaddrin.sin_addr, hostp->h_addr, sizeof(clientaddrin.sin_addr));
	}
	else 
		printf("server HOST/ IP %s, port = 9999\n","localhost");
		


	if(connect(clientSoc, (struct sockaddr *) &clientaddrin, sizeof(clientaddrin)) != 0)
	{
		perror("client connecting to server connect failed ");
		return -1;
	}
	system("stty raw");
	while(1)
	{
		memset(msg,0,100);
		printf("write message to send \n");
		//ch = getchar();
		scanf("%c",&ch);
		printf("\ngetchar = %d\n",ch);
		if(ch == ';')
		{
		   system("stty cooked");
		   break;	
		}
		//putchar(ch);
		//scanf("%s",msg);
		if(!strcmp(msg,"exit"))
		{
			break;
		}
	
		if(send(clientSoc, &ch,1/*strlen(msg)*/,0) < 0)
		{
			printf("Send failed\n");
			return -1;
		}
	//	printf(" Send msg server success \n");
#if 1
		if(recv(clientSoc, msg, 100, 0) < 0)
		{
			printf(" receive failed \n");
		}
		else
			printf("Received msg = %s \n\n",msg);
#endif
	}

	close(clientSoc);
	/* send Receive */
}
