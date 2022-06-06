#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netdb.h>
#include <fcntl.h> 
#include <unistd.h>

int main(){

    int sockfd, clen, clientfd;
    struct sockaddr_in saddr, caddr;
    unsigned short port = 8784;
    char clientms[1000], svms[1000];


    if ((sockfd =  socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);


    if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0){
        printf("Error binding\n");  
        return 1;
    }

    if (listen(sockfd, 5) < 0){
        printf("Error listening\n");  
        return 1;
    }

 
    clen = sizeof(caddr);
    if ((clientfd = accept(sockfd, (struct sockaddr *) &caddr, &clen)) < 0) {
        printf("Error accepting connection\n"); 
        return 1;
    }
    printf("Connection Accepted \n");
    
    while (1){
        printf("Client: ");
        
        do{
            recv(clientfd, clientms, sizeof(clientms), 0);
            printf("%s", clientms);
            if (strcmp(clientms,"/quit\n\0")==0){
                printf("Client disconected\n");
            }
        } while (clientms[strlen(clientms) - 1] != '\n');
        

        printf("Message to client: ");
        
        do{
            fgets(svms, 1000, stdin);
            if (strcmp(svms,"/dc\n\0")==0){
                shutdown(sockfd,SHUT_RDWR);
                shutdown(clientfd, SHUT_RDWR);
                printf("Server disconnected\n");
                close(sockfd);
                close(clientfd);
                return 0;

            }
            send(clientfd, svms, strlen(svms) + 1, 0);
        } while (svms[strlen(svms) - 1] != '\n');

    }
    return 0;
}
