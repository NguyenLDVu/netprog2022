#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]){

    char input[200];
    struct in_addr *address;
    printf("Enter host domain name: ");
    scanf("%s", input);

    struct hostent *host = gethostbyname(input);
    if (host == NULL ) {
        printf("Cannot find host address \n");
        exit(1);
    }
    else {  
       for ( int i = 0; host->h_addr_list[i] != NULL; i++ ){
        	address = (struct in_addr *)(host->h_addr_list[i]);
        	printf("Address : %s \n", inet_ntoa(*address));
    	}	
    }
}
