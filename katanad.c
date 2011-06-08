/*
 * project:         katana
 * part:            katanad - the katana master process
 * author:          Manuel Fill (ap0calypse@agitatio.org)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/wait.h>
#include <netdb.h>

/*
 * global definitions, port, backlog etc...
*/


#define KATANA_PORT 13337
#define BACKLOG 10

int main(void) {
    char buffer[1024];
    printf("katanad starting up ...\n");
    
    
    // define the sockaddress stuff
    struct sockaddr_in kat_master, kat_slave;
    int yes = 1;
    memset(&kat_master, 0, sizeof(kat_master));
    kat_master.sin_family = AF_INET;
    kat_master.sin_port = htons(KATANA_PORT);
    kat_master.sin_addr.s_addr = htonl(INADDR_ANY);

    int master_sock, slave_sock, n;
    printf("creating socket ... ");
    master_sock = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(master_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (master_sock > 0) {
        printf("ok\n");
    }
    else {
        printf("failed\n");
        exit(255);
    }

    printf("binding address to socket ... ");
    if (bind(master_sock,(struct sockaddr *)&kat_master, sizeof(kat_master)) != -1) {
        printf("ok\n");
    }
    else {
        printf("failed\n");
    }

    printf("creating listening socket ...");
    if (listen(master_sock, BACKLOG) != -1) {
        printf("ok\n");
    }
    else {
        printf("failed\n");
    }

    while (1) {
        socklen_t len_inet = sizeof(kat_slave);
        char gna[1024];
        slave_sock = accept(master_sock, (struct sockaddr *)&kat_slave, &len_inet);
        inet_ntop(AF_INET, &kat_slave.sin_addr, gna, 1024);
        printf("got connection from %s\n", gna); 
        n = sprintf(buffer, "welcome to katana\n");
        write(slave_sock, buffer, n);
        close(slave_sock);
    }
    return 0;
}
