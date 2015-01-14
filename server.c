#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>



#ifndef null
#define null 0
#endif

int main(int argc,char** arg){
    
    socklen_t clilen;
    int sockfd,newsock;
    struct sockaddr_in server;
    int client;
    const int FD_SIZE=16;

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd == -1){
        printf("open socket failed");
        return 1;
    }
    memset(&server,sizeof(server),0);

    server.sin_family=AF_INET;
    server.sin_port = htons(5000);
    
    /*
    int ret = inet_aton("0.0.0.0",&server);
    if(ret == -1){
        printf("cp internet address failed!");
        return 1;
    }
*/
    server.sin_addr.s_addr = inet_addr("0.0.0.0");
    int addrLen = sizeof(server.sin_addr);
    bind(sockfd,(struct sockaddr *)&server,sizeof(server));
    
    listen(sockfd,64);
    
    /***
     *select
     */
    fd_set rdfs;
    fd_set activefs;

    struct timeval tv;
    int retval;

    FD_ZERO(&activefs);
    FD_SET(sockfd,&activefs);

    tv.tv_sec = 5;
    tv.tv_usec = 0;

    while(1){
    printf("start server, waiting for connection..\n");
    fflush(stdout);
    rdfs = activefs;
    retval = select(1,&rdfs, NULL,NULL,&tv);

    if(retval == -1){
        printf("error fd");
    }else if(retval){
        for(int i=0;i<FD_SETSIZE;i++){
            printf("be ready to recv data");
            if(FD_ISSET(sockfd,&rdfs)){
                       
                /*
                printf("start server, waiting for connection..\n");
                fflush(stdout);
                */
                client = accept(sockfd,(struct sockaddr *)null,(socklen_t *)null);
                if(client == -1){
                    continue;
                }
                printf("accept client %d \n",client);
                
                FD_SET(client,&activefs);

            }else{
                client = i;//file description
                char buf[128];
                memset(buf,0,sizeof(buf));

                ssize_t rcvd = recv(client,buf,sizeof(buf),0);
                if(rcvd >0){
                    printf("recv data %d bytes, value : %s\n",(int)rcvd, buf);
                }
                memset(buf,0,sizeof(buf));

                memcpy(buf,"hello girl",strlen("hello girl"));

                send(client,buf,strlen("hello girl\0"),MSG_NOSIGNAL);

                close(client);
                FD_CLR(client,&activefs);
            }
        }
    }
}

    close(sockfd);
}
