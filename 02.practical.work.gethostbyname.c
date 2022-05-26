#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    char input[200];

    printf("Enter host domain name: ");
    scanf("%s", input);

    if (input == NULL ) {
        printf("invalid input");
        exit(1);
    }

    struct hostent *host = gethostbyname(input); 
    struct in_addr *address;

    for ( int i = 0; host->h_addr_list[i] != NULL; i++ ){
        address = (struct in_addr *)(host->h_addr_list[i]);
        printf("Address : %s \n", inet_ntoa(*address));
    }

}
