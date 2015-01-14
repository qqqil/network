#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>



int main(int argc, char** argv){

    int fd;
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    
    //inet_aton("172.18.52.252",&server.sin_addr);
    server.sin_addr.s_addr=inet_addr("172.18.52.252");

    fd = socket(AF_INET,SOCK_STREAM,0);

    if(fd == -1){
        printf("create socket failed!");
        return -1;
    }
    
    int ret = connect(fd,(struct sockaddr *) &server, sizeof(server));
    
    if(ret == -1){
        printf("connect to server failed!Error Code:%d",ret);
        if(fd != -1){
            close(fd);
        }
        return -1;
    }
    char *buf="hello world!";
    int len = strlen(buf);
    ssize_t snd = send(fd,buf,len,0);

    printf("send data to server for %d bytes \n",(int)snd);
    char data[512];
    memset(data,0,sizeof(data));
    ssize_t rcvd = recv(fd,data,sizeof(data),0);

    if(rcvd >0){
        printf("recv data from server %d bytes, value : %s \n",(int)rcvd,data);
    }
    close(fd);

    return 0;
}
