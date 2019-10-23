#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAXDATASIZE 1400

int DNSquery(char* hostname)
{
	char  *ptr;
	struct hostent *hostptr;

	if ((hostptr = gethostbyname(hostname)) == NULL)
	{
		printf("DNS query failed.\n");
		return -1;
	}

	if(hostptr->h_addrtype == AF_INET)
	{
		printf("Response from %s \n", inet_ntop(hostptr->h_addrtype, *(hostptr->h_addr_list), hostname, 64));

	}else{
		printf("DNS query failed.\n");
        return -1;
	}
    return 0;
}

int main(int argc, char *argv[])
{
    float time_sec;
    int sockfd, num, pf, nameLen=0, min=0, hr=0;
    unsigned char buf[2048],challenge[9],request[9],*p_time = (unsigned char*)&time_sec;
    
    //argments check
    if(argc != 2)
    {
        printf("\nUsage: %s\033[31;1m IP_address:\033[32mPort \033[0m(port is optional, use 27015 by default)\n\n", argv[0]);
        return -1;
    }

    //transfer arguments to declare ip,port and socket
    char *server_IP, *getport, *input_option=strdup(argv[1]);
    server_IP = strsep(&input_option,":");
    getport = strsep(&input_option,":");
    free(input_option);
    int PORT=(getport==NULL)?27015:atoi(getport);
    struct sockaddr_in server;

    //define protocol as UPD and initiate the socket
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket() error\n");
        return -1;
    }
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if((server.sin_addr.s_addr = inet_addr(server_IP))==-1)
    {
        if(DNSquery(server_IP)==-1) return -1;
        server.sin_addr.s_addr = inet_addr(server_IP);
    }

    //set timeout limit to avoid stuck at recv() process
    struct timeval timeout;
    timeout.tv_sec = 3; timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        printf("setsockopt() error.\n");
        return -1;
    }

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Connection Failed.\n");
        return -1;
    }

    //Send query request to Valve server
    for(pf=0;pf<9;pf++) request[pf]=0xFF;
    request[4]=0x55;
    send(sockfd, request, strlen(request), 0);

    //Receive challenge code from server
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
        printf("Server Time Out.\n");
        return -1;
    }

    //Reply the challenge
    for(pf=0;pf<num;pf++) challenge[pf]=buf[pf];
    challenge[4]=request[4];    
    send(sockfd, challenge, strlen(challenge), 0);

    //Receive Players info
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
        printf("Server Time Out.\n");
        return -1;
    }
    putchar('\n');
        
    //Decode the bytes and print
    printf("Total Players: %d\n",buf[5]);
    pf=6;nameLen=0;
    while(pf<num)
    {
        if(buf[pf]==0x00) pf++;
        //Print player name
        while(buf[pf]!=0x00)
        {
            putchar(buf[pf++]);
            nameLen++;
        }
        if(nameLen==0) printf("Loading");
        //Print score
        printf("\t%d\t",buf[++pf]);pf+=4;
        //Print time as float
        p_time[0]=buf[pf];p_time[1]=buf[pf+1];p_time[2]=buf[pf+2];p_time[3]=buf[pf+3];
        hr=time_sec/3600;min=(time_sec-hr*3600)/60;
        if(hr) printf("%dh",hr);
        if(min) printf("%dm",min);
        printf("%.0fs\n",time_sec-3600*hr-60*min);pf+=4;nameLen=0;
    }
    putchar('\n');

    close(sockfd);
return 0;
}
