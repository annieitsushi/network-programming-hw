
// For a server with more than one network cards / IP addresses - enter the IP address to bind the server socket to.
// Example:
// ./TCP_DNSserver.out 192.168.1.2
// If no IP address is entered as a parameter, binds to all IP addresses of the server.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "string.h"

struct in6_addr my_addr;
struct sockaddr_in6 my_sock;

int my_fd, new_fd, numbytes;
char buf[255];
char ans[4096];
//struct hostent *query;
#define PORT "3333";

int main(int argc, char *argv[])
{
	int my_fd=socket(PF_INET6,SOCK_STREAM,0);
		if(my_fd==-1)
		{
			perror("socket");
			return 1;
		}

		memset(&my_sock, 0, sizeof(my_sock));
		if(argc==1)
		{
			my_sock.sin6_family = PF_INET6;
			my_sock.sin6_addr = in6addr_any;
			my_sock.sin6_port = htons(3333); //PORT;
		}
		else{
			if (inet_pton(AF_INET6, argv[1], &(my_sock.sin6_addr)) != 1) {

				fprintf(stderr, "Invalid address\n");
				return 1;
			}
			my_sock.sin6_family = PF_INET6;
			my_sock.sin6_port = htons(3333); //PORT;

			char ipinput[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &(my_sock.sin6_addr), ipinput, INET6_ADDRSTRLEN);
			printf("IP Address = %s\n", ipinput);

		}
		if( bind(my_fd, (struct sockaddr *)&my_sock, sizeof my_sock) == -1)
		{
			perror("bind");
			return 1;
		}
		//sleep(15);
		if (listen(my_fd, 10) == -1)
		{
			perror("listen");
			return 1;
		}
		struct sockaddr_storage their_addr;
		socklen_t sin_size = sizeof(their_addr);

		while(1)
		{
			new_fd = accept(my_fd, (struct sockaddr *)&their_addr, &sin_size);
			if (new_fd == -1) {
				perror("accept");
				return 1;
			}
		if (!fork()) // this is the child process
    {
			close(my_fd); // child doesn't need the listener
			if ((numbytes = recv(new_fd, buf, 254, 0)) == -1)
      {
				perror("recv");
				exit(1);
			}
//			query=gethostbyname(buf);
//			inet_ntop(query->h_addrtype, query->h_addr_list[0], ans,  sizeof ans);
			struct addrinfo hints, *servinfo4, *servinfo6;
      int rv;
      memset(&hints, 0, sizeof(hints));

			hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
	    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	    hints.ai_protocol = 0;          /* Any protocol */
	    hints.ai_canonname = NULL;
	    hints.ai_addr = NULL;
	    hints.ai_next = NULL;
      if ((rv = getaddrinfo(buf, NULL, &hints, &servinfo4)) != 0)
      {
   			  fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
   			  return 1;
      }
			for (struct addrinfo * si = servinfo4; si != NULL; si = si->ai_next)
			{
				char curAns[256] = "";
				struct sockaddr_in * first = (struct sockaddr_in *)(si->ai_addr);
				inet_ntop(si->ai_family, &(first->sin_addr), curAns, sizeof curAns);
	      strcat(curAns,"\n");
	      strcat(ans,curAns);
				printf("resolved address: %s", curAns);
			}

			//separator for IPv4 and IPv6
			strcat(ans,"*");

			hints.ai_family = AF_INET6;
      if ((rv = getaddrinfo(buf, NULL, &hints, &servinfo6)) != 0)
      {
   			  fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
   			  return 1;
      }
			for (struct addrinfo * si = servinfo6; si != NULL; si = si->ai_next)
			{
				char curAns[256] = "";

	      struct sockaddr_in6 * first = (struct sockaddr_in6 *)(si->ai_addr);
	      inet_ntop(si->ai_family, &(first->sin6_addr), curAns, sizeof curAns);

	      strcat(curAns,"\n");
	      strcat(ans,curAns);
				printf("resolved address: %s", curAns);
			}
			if (send(new_fd, ans, 4096, 0) == -1)
				perror("send");


			close(new_fd);
			exit(0);
		}
	}
	//	close(my_fd);
	//	return 0;
}
