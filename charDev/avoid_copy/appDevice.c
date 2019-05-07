/**
 *  * @file   testebbchar.c
 *   * @author Derek Molloy
 *    * @date   7 April 2015
 *     * @version 0.1
 *      * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 *       * string to the LKM and reads the response from the LKM. For this example to work the device
 *        * must be called /dev/ebbchar.
 *         * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
 *         */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include <sys/mman.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main(){
	int ret, fd;
	char stringToSend[BUFFER_LENGTH];
	printf("Starting device test code example...\n");
	fd = open("/dev/aatgc", O_RDWR);             // Open the device with read/write access
	if (fd < 0){
		perror("Failed to open the device...");
		return errno;
	}
#if 0
	printf("Type in a short string to send to the kernel module:\n");
	scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)
	printf("Writing message to the device [%s].\n", stringToSend);
	ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
	if (ret < 0){
		perror("Failed to write the message to the device.");
		return errno;
	}

	printf("Press ENTER to read back from the device...\n");
	getchar();

	printf("Reading from the device...\n");
	ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
	if (ret < 0){
		perror("Failed to read the message from the device.");
		return errno;
	}
	printf("The received message is: [%s]\n", receive);
#endif
        int* mmappedData = NULL;
	short i = 0;
	mmappedData = (int *) mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);
	if (mmappedData == NULL)
	{
	  printf("Mmap failed \n");
	  return -1;
	}
	else 
	  printf("Mmap pass, Address = %p\n",mmappedData);
#if 1	
	for (i = 0; i < 4096; i++)
	{
	  mmappedData[i] = i;
	}

	for (i = 0; i < 4096; i++)
	{
	  printf("%d ",mmappedData[i]);
	}
	printf("\n");
	
	ret = write(fd, mmappedData, 4096); // Send the string to the LKM
	if (ret < 0){
		perror("Failed to write the message to the device.");
		return errno;
	}
#endif
	printf("End of the program\n");
	return 0;
}
