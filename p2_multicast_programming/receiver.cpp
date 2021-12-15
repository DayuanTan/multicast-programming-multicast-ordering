#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <cstdlib>
#include <ctime>
#include <vector>

#define PORT 5555

using namespace std;




   
int main(int argc, char const *argv[])
{

    
    printf("Receiver starts. Receiver pid is %d \n", getpid());

    int receiver_socket_fd, valread;
    char receiver_read_buffer[1024] = {0};

    

    // // Creating socket file descriptor (IPv4, TCP, IP)
    // if ((receiver_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    // For multicast it must be family AF_INET, and its type may be either SOCK_DGRAM or SOCK_RAW. The most common use is with SOCK_DGRAM sockets
    if ((receiver_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) // (IPv4, UDP, IP)
    {
        perror("Receiver: Socket creation error \n");
        exit(EXIT_FAILURE);
    }


    // Optional: it helps in reuse of address and port. Prevents error such as: “address already in use”.
    int opt = 1; // for setsockopt
    if (setsockopt(receiver_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
        perror("Receiver: setsockopt SOL_SOCKET");
        exit(EXIT_FAILURE);
    }



    /* Bind to the proper port number with the IP address specified as INADDR_ANY. */
    struct sockaddr_in receiver_addr;
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr = INADDR_ANY; //is localhost
    receiver_addr.sin_port = htons(PORT); 
    // Forcefully attaching socket to the port 8080
    if (bind(receiver_socket_fd, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr))<0)
    {
        perror("Receiver: bind failed");
        exit(EXIT_FAILURE);
    } 
    

    /* Join the multicast group 226.1.1.1 on the local interface 192.168.169.154 interface. */
    /* Note that this IP_ADD_MEMBERSHIP option must be */
    /* called for each local interface over which the multicast */
    /* datagrams are to be received. */
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
    group.imr_interface.s_addr = inet_addr("192.168.169.154");
    if (setsockopt(receiver_socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
    {
        perror("Receiver: Adding multicast group error");
        exit(EXIT_FAILURE);
    }
    
    // receiving form multicast
    if (read(receiver_socket_fd , receiver_read_buffer, 1024) < 0){
        perror("Receiver: read() error");
        exit(EXIT_FAILURE);
    }
    printf("Receiver: read: '%s'\n",receiver_read_buffer );

       

    close(receiver_socket_fd);
    return 0;
}