#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "net.h"
#include "leaderboard.h"
#include "snake.h"

int main(void){

    int socketfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd1 == -1){
        return 0;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(PORT);

    int result = connect(socketfd1, (struct sockaddr *)&addr, sizeof(addr));
    if (result == -1){
        return 0;
    }

    //test to see if the client can receive the message
    char buffer[64];
    int bytes_received = recv(socketfd1, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1){
        return 0; 
    }
    buffer[bytes_received] = '\0';
    printf("Received: %s\n", buffer);

    return 0;
}
