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
		printf("DNS query failure: unable to solve %s\n",hostname);
		return -1;
	}

	if(hostptr->h_addrtype == AF_INET)
	{
		inet_ntop(hostptr->h_addrtype, *(hostptr->h_addr_list), hostname, 64);
	}else{
		puts("DNS query failure: unknown address type\n");
        return -1;
	}
    return 0;
}

int main(int argc, char *argv[])
{
    float time_sec;
    int sockfd, num, pf, nameLen=0, min=0, hr=0;
    unsigned char buf[2048],challenge[9],player_request[9],*p_time = (unsigned char*)&time_sec;
    unsigned char info_request[29]={0xFF,0xFF,0xFF,0xFF,0x54,0x53,0x6F,0x75,0x72,0x63,0x65,0x20,0x45,0x6E,0x67,0x69,0x6E,0x65,0x20,0x51,0x75,0x65,0x72,0x79,0x00,0xff,0xff,0xff,0xff};
    
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
        puts("socket() error\n");
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
        puts("setsockopt() error.\n");
        return -1;
    }

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        puts("Connection Failed.\n");
        return -1;
    }

    //Send info request to Valve server
    send(sockfd, info_request, 29, 0);
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
        printf("%s time out.\n",server_IP);
        return -1;
    }

    //in the case of server return a challeng number
    if(buf[4] & 0x41){
	info_request[25]=buf[5];info_request[26]=buf[6];info_request[27]=buf[7];info_request[28]=buf[8];
	send(sockfd, info_request, 29, 0);
	
	if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    	{
		printf("%s time out.\n",server_IP);
		return -1;
	}
    }

    //Print server basic info
    for(pf=6;;)
    {
        while(buf[pf]!=0x00) putchar(buf[pf++]);
        putchar('\t');pf++;
        while(buf[pf]!=0x00) putchar(buf[pf++]);
        putchar('\t');pf++;
        while(buf[pf++]!=0x00);while(buf[pf++]!=0x00);
        pf+=2;
        printf("%d/%d\t%s:%d\n",buf[pf],buf[pf+1],server_IP,PORT);
        break;
    }

    //Send player request to Valve server
    for(pf=0;pf<9;pf++) player_request[pf]=0xFF;
    player_request[4]=0x55;
    send(sockfd, player_request, 9, 0);

    //Receive challenge code from server
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
        printf("%s time out.\n",server_IP);
        return -1;
    }

    //Reply the challenge
    for(pf=0;pf<num;pf++) challenge[pf]=buf[pf];
    challenge[4]=player_request[4];    
    send(sockfd, challenge, 9, 0);

    //Receive Players info
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
        printf("%s time out.\n",server_IP);
        return -1;
    }
    //putchar('\n');
        
    //Decode the bytes and print
    //printf("Total Players: %d\n",buf[5]);
    if(buf[5]>0x00)
    {
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
    }

    close(sockfd);
return 0;
}
