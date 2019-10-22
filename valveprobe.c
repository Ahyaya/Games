#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXDATASIZE 1400

int main(int argc, char *argv[])
{
    int sockfd, num, pf, nameLen=0, min=0, hr=0;
    unsigned char buf[2048],challenge[9],request[9];
    float time_sec;
    unsigned char *p_time = (unsigned char*)&time_sec;
    
    if(argc != 2)
    {
        printf("Usage: %s\033[31;1m IP_address:\033[32mPort \033[0m(port is optional, use 27015 by default)\n\n", argv[0]);
        exit(1);
    }

    char *server_IP, *getport, *input_option=strdup(argv[1]);
    server_IP = strsep(&input_option,":");
    getport = strsep(&input_option,":");
    free(input_option);
    int PORT=(getport==NULL)?27015:atoi(getport);
    struct sockaddr_in server;

    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket() error\n");
        exit(1);
    }
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    server.sin_addr.s_addr = inet_addr(server_IP);
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("connect() error.\n");
        exit(1);
    }
    for(pf=0;pf<9;pf++) challenge[pf]=0xFF;
    challenge[4]=0x55;
    send(sockfd, challenge, strlen(challenge), 0);

    while(1)
    {
        if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
        {
            printf("recv() error.\n");
            exit(1);
        }
        printf("Server challenge:\t");
        for(pf=0;pf<num;pf++)
        {
            printf("%02x ",buf[pf]);
        }
        putchar('\n');
        break;
    }
    for(pf=0;pf<4;pf++) request[pf]=0xFF;
    request[4]=0x55;//This parameter decide which session to consult.
    for(pf=5;pf<9;pf++) request[pf]=buf[pf];
    printf("\nSending request:\t");
    for(pf=0;pf<9;pf++)
    {
        printf("%02x ",request[pf]);
    }
    send(sockfd, request, strlen(request), 0);
    putchar('\n');
    while(1)
    {
        if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
        {
            printf("recv() error.\n");
            exit(1);
        }
        printf("\nServer reply:\n");
        for(pf=5;pf<num;pf++)
        {
            printf("%02x ",buf[pf]);
        }
        putchar('\n');
        break;
    }
    printf("Total Players: %d\n",buf[5]);
    pf=6;nameLen=0;
    while(pf<num)
    {
        if(buf[pf]==0x00) pf++;
        while(buf[pf]!=0x00){putchar(buf[pf++]);nameLen++;}
        if(nameLen==0) printf("Loading");
        printf("\t%d\t",buf[++pf]);pf+=4;
        p_time[0]=buf[pf];p_time[1]=buf[pf+1];p_time[2]=buf[pf+2];p_time[3]=buf[pf+3];
        hr=time_sec/3600;min=(time_sec-hr*3600)/60;
        if(hr) printf("%dh",hr);
        if(min) printf("%dm",min);
        printf("%.0fs\n",time_sec-3600*hr-60*min);
        pf+=4;
        nameLen=0;
    }
    putchar('\n');

    close(sockfd);
}
