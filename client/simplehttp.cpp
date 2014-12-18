#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#if defined (_WIN32)

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib"))

#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 


#endif

#include "simplehttp.h"

/* Some code is adapted from http://www.linuxhowtos.org/C_C++/socket.htm */

void error(const char *msg) {
#if defined (_WIN32)
	printf("%s: %d\n", msg, WSAGetLastError());
#else
	perror(msg);
#endif
    exit(0);
}


/* Returns a file descriptor of a connected socket.
   Notice that under windows, WSAStartup should be called before calling this.*/
int connectTCP(const char* host, const char* page, int portno) {
    int sockfd;
   	
	struct sockaddr_in serv_addr;
    struct hostent *server;

	// Create socket //
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) 
        error("ERROR opening socket");
    

	// Get ip from hostname //
	server = gethostbyname(host);
    if (server == NULL)
        error("ERROR, can not find host\n");
    
	memset((char *) &serv_addr, 0, sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, // Destination
			(char *)server->h_addr,				// Source
			server->h_length);					// Size
    serv_addr.sin_port = htons(portno);

	// Conect to socket //
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
	return sockfd;

}


char* httpGet(const char* host, const char* page, int portno) {
  int sockfd, n;
  char* buffer = (char*)calloc(BUFFER_MAX_SIZE + 1, sizeof(char));

#if defined (_WIN32)
  WSADATA wsadata;
  if(WSAStartup(MAKEWORD(2,2), &wsadata) != NO_ERROR)
    error("ERROR during WSAStartup");
#endif
	
	sockfd = connectTCP(host, page, portno);
    memset(buffer, 0, sizeof(buffer));
    
	sprintf(buffer, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n", page, host);
	//printf("%s", buffer);

	n = send(sockfd,buffer,strlen(buffer), 0);
	
    if (n < 0) 
         error("ERROR writing to socket");
   
	int count = 0;
	do {
		n = recv(sockfd, buffer + count, BUFFER_MAX_SIZE, 0);
		if (n < 0) 
			error("ERROR reading from socket");

		count += n;
	} while(n != 0);

#if defined (_WIN32)
	closesocket(sockfd);
	WSACleanup();
#else
	close(sockfd);
#endif


	return buffer;
}


char* extractContent(char* response) {
	char* i = strstr(response, "\r\n\r\n");
	if (i == NULL)
		return NULL;

	return i+4;
}

