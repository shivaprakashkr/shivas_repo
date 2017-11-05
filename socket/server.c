#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<string.h>
#include <unistd.h>

int main()
{
	int serverSocId = 0;
	int clientSocId = 0;
	struct sockaddr_in sockaddrin;
	struct sockaddr_in client;
	struct sockaddr_storage storage;
	size_t socStorageSize = 0;
	int ret = 0;
	int readSize = 0;
	char message[100];
	char *msg;
	/* Create socket */
	/* Socket charecterized based on 
	 * 1. Domine(address family AF).
	 * 2. Type(virtual circuit (TCP) DATAGRAM (UDP)).
	 * 3. Protocal (TCP or UDP) 0 -> TCP (Default).
	 */
	serverSocId = socket(AF_INET, SOCK_STREAM, 0);
	if(0 == serverSocId)
	{
		perror("Socket creation failed ");
		return -1;
	}
	/* bought the device :) */
	bzero(&sockaddrin, sizeof(sockaddrin));
	sockaddrin.sin_family = AF_INET;
	sockaddrin.sin_port = htons(9999);
	sockaddrin.sin_addr.s_addr = htonl(INADDR_ANY); /* Any internet address */

	printf(" server soc address %d \n",inet_addr("08:00:27:4f:b3:8d"));

	/* Bind the socket to inet address */
	ret = bind (serverSocId,(struct sockaddr *) &sockaddrin, sizeof(sockaddrin));
	if(ret < 0)
	{
		perror("Server Bind error ");
		return -1;
	}

	printf(" bind success  \n");
	/* Took number to device */
	while(1)
	{
		/* Listen to the clients to connect */
		if(0 == listen(serverSocId, 5))
		{
			printf("Listening ...\n");
		}
		else
			perror("Start listen ");

		/* Waiting for call */

		/* Accept the request from clients */
		//while(clientSocId = accept(serverSocId, (struct sockaddr_in *) &client, sizeof(client)) < 0)
		int len = sizeof(storage);
		clientSocId = accept(serverSocId, (struct sockaddr *) &storage,(socklen_t *) &len);
		{
			if (0 >= clientSocId)
			{
				perror("accept failed ");
				return -1;//break;
			}

			printf(" Accept pass: client Id = %d \n",clientSocId);
			memset(message,'\0',100);
			while((readSize = recv(clientSocId, message, 1 /*sizeof(message)*/,0)) > 0)
			{
				printf("Message = %c, %d\n",message[0],message[0]);
			//else 
				//printf (" serve Receive failed \n");
				//memcpy(message, "AATGC", 6);
				msg = "AATGC";
				write(clientSocId, message, strlen(message)); 
				printf("Server Send message suucess \n\n");
			}
			/* Received call */

			/* send or receive any data here */
			//if(recv
			//readSize = recv(clientSocId, message, 99 /*sizeof(message)*/,0);
			//printf("Message = %s\n",message);
		}
	}
	close(clientSocId);
	close(serverSocId);

}




