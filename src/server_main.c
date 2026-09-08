#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "net.h"
#include "snake.h"
#include "leaderboard.h"


int main(void){

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1){
        return 0;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(socketfd, (struct sockaddr *)&addr, sizeof(addr));

    listen(socketfd, 2);

    int client1_fd = accept(socketfd, NULL, NULL);
    if (client1_fd == -1){
        return 0;
    }
    printf("Player 1 connected. Waiting for player 2...\n");

    int client2_fd = accept(socketfd, NULL, NULL);
    if(client2_fd == -1){
        return 0;
    }

    //providing raw send/recv actually works between them using simple messages before moving on to the real game data
    char *msg = "Hello from server";
    send(client1_fd, msg, strlen(msg), 0);

    return 0;
}
