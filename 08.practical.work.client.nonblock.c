#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netdb.h>
#include <fcntl.h> 
#include <unistd.h>
#include <poll.h>

int main(){

    setbuf(stdout, NULL);

    char input[200];
    struct sockaddr_in saddr;
    struct hostent *h;
    int sockfd;
    unsigned short port = 8784;
    char clientms[1000], svms[1000];

    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error creating socket \n");
        return 1;
    }
    
    struct in_addr *address;
    printf("Enter host domain name: ");
    fgets(input, 200, stdin);
    input[strlen(input) - 1] = '\0';

    if ((h=gethostbyname(input)) == NULL) {
        printf("Unknown host \n");
        return 1;
    }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    saddr.sin_port = htons(port);

    setsockopt(sockfd, SOL_SOCKET,
               SO_REUSEADDR, &(int){1},
               sizeof(int));

    if (connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        printf("Cannot connect\n");
        return 1;
    }
    printf("Connected to server successfully \n");

    int sockfl = fcntl(sockfd, F_GETFL, 0);
    sockfl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, sockfl);

    struct pollfd pollfd;
    pollfd.fd = fileno(stdin);
    pollfd.events = POLLIN;
    
    int notfinish = 1;
    printf("Message to server: ");

    while (notfinish)
    {
        if (recv(sockfd, svms, sizeof(svms), 0) > 0)
        {
            printf("\rServer: ");
            printf("%s", svms);
            printf("Message to server: ");
        }
        if (poll(&pollfd, 1, 0) > 0)
        {
            if (pollfd.revents & POLLIN)
            {
                fgets(clientms, sizeof(clientms), stdin);
                if ((strcmp(clientms, "/quit\n")) == 0) {
                    printf("Disconnect to server\n");
                    return 0;
                }
                send(sockfd, clientms, strlen(clientms) + 1, 0);
                printf("Message to server: ");
            }
        }
    }
    close(sockfd);

    return 0;
}
