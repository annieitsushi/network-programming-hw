#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3333" // the port client will be connecting to

#define MAXDATASIZE 255 // max number of bytes we can get at once


int main(int argc, char *argv[])
{
	int my_fd, numbytes;
	char buf[MAXDATASIZE];
	struct sockaddr_in6 my_sock;


	if (argc != 3) {
		fprintf(stderr,"usage: client serverIP hostname_to_resolve\n");
		exit(1);
	}

	my_fd=socket(PF_INET6,SOCK_STREAM,0);
	if(my_fd==-1){
		perror("socket");
		return 1;
	}

	my_sock.sin6_family = PF_INET6;
	if (inet_pton(AF_INET6, argv[1], &(my_sock.sin6_addr)) != 1) {
			fprintf(stderr, "Invalid address\n");
			return 1;
		}
	my_sock.sin6_port = htons(3333);

	if (connect(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1) {
		close(my_fd);
		perror("client: connect");
		return 1;
	}

	if (send(my_fd, argv[2], 255, 0) == -1)
	perror("send");



	if ((numbytes = recv(my_fd, buf, MAXDATASIZE-1, 0)) == -1)
	{
		perror("recv");
		exit(1);
	}

	//printf("HERE%s%d\n",buf,numbytes);
	buf[numbytes] = '\0';

	char IPv4[4096]="",IPv6[4096]="",str[256]="";
	int count4 = 0, count6 = 0;
	char *ptrBuf = buf;

		while(ptrBuf[0] != '*' && sscanf(ptrBuf, "%s[^\n]", str) > 0)
		{
			//printf("client: received '%s'\n",str);
			strcat(IPv4, str);
			strcat(IPv4, "\n");
			count4++;
			ptrBuf = strchr(ptrBuf, '\n') + 1;
		}
		if (!count4)
		{
			printf("No IPv4 addressses resolved.\n");
		}
		else
		{
			char* ptrIP4 = IPv4;
			printf("A total of %d IPv4 addressses resolved:\n", count4);
			while(sscanf(ptrIP4, "%s[^\n]", str) > 0)
			{
				printf("PF_INET resolved address: '%s'\n",str);
				ptrIP4 = strchr(ptrIP4, '\n') + 1;
			}
		}

		ptrBuf++; //skip
		while(sscanf(ptrBuf, "%s[^\n]", str) > 0)
		{
			strcat(IPv6, str);
			strcat(IPv6, "\n");
			count6++;
			ptrBuf = strchr(ptrBuf, '\n') + 1;
		}
		if (!count6)
		{
			printf("No IPv6 addressses resolved.\n");
		}
		else
		{
			char* ptrIP6 = IPv6;
			printf("A total of %d IPv6 addressses resolved:\n", count6);
			while(sscanf(ptrIP6, "%s[^\n]", str) > 0)
			{
				printf("AF_INET6 resolved address: '%s'\n",str);
				ptrIP6 = strchr(ptrIP6, '\n') + 1;
			}
		}


	close(my_fd);

	return 0;
}
