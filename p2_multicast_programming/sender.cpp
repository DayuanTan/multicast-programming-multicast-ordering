#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#define PORT 5555

using namespace std;



 
int main(int argc, char *argv[])
{
    // /* deal with input arguments*/
    // std::cout << "print arguments:\nargc == " << argc << '\n';
    // for(int ndx{}; ndx != argc; ++ndx) {
    //     std::cout << "argv[" << ndx << "] == " << argv[ndx] << '\n';
    // }
    // std::cout << "argv[" << argc << "] == "
    //           << static_cast<void*>(argv[argc]) << '\n';
    
   
    printf("Sender starts. Sender pid is %d \n", getpid());

    

    // Socket Cite: https://www.geeksforgeeks.org/socket-programming-cc/?ref=lbp
    // Multiocast Cite: https://www.tenouk.com/Module41c.html
    int sender_socket_fd, new_socket;

    
    /* Initialize the multicast group sockaddr structure with a */
    /* group address of 226.1.1.1 and port 5555. */
    struct sockaddr_in group_address;
    group_address.sin_family = AF_INET;    // IPv4
    group_address.sin_addr.s_addr = inet_addr("226.1.1.1"); //  = INADDR_ANY is localhost
    group_address.sin_port = htons( PORT ); // 8080
    


    // // Creating socket file descriptor (IPv4, TCP, IP)
    // if ((sender_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    // For multicast it must be family AF_INET, and its type may be either SOCK_DGRAM or SOCK_RAW. The most common use is with SOCK_DGRAM sockets
    if ((sender_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) // (IPv4, UDP, IP)
    {
        perror("Sender: socket failed");
        exit(EXIT_FAILURE);
    }
       


    // Optional: it helps in reuse of address and port. Prevents error such as: “address already in use”.
    int opt = 1; // for setsockopt
    if (setsockopt(sender_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
        perror("Sender: setsockopt SOL_SOCKET");
        exit(EXIT_FAILURE);
    }
    // For multicast, the level (2nd param) should be IPPROTO_IP which has multiple options. 
    // IP_MULTICAST_LOOP: 0 if you want the data you send to be looped back to your host, 1 if not. Default is 0. We don't need to change.
    // IP_MULTICAST_TTL: Sets the Time To Live (TTL) in the IP header for outgoing multicast datagrams. 0 samehost. 1 same subnet. Default is 1. 
    u_char ttl = '0'; // because we use only one machine 
    if (setsockopt(sender_socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0)                                              
    {
        perror("Sender: setsockopt IPPROTO_IP IP_MULTICAST_TTL");
        exit(EXIT_FAILURE);
    }
    /* Set local interface for outbound multicast datagrams. */
    /* The IP address specified must be associated with a local multicast capable interface. */
    struct in_addr localInterface;
    localInterface.s_addr = inet_addr("192.168.169.154"); // On linux use ifconfig to check "UP ... MULTICAST"
    if(setsockopt(sender_socket_fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
    {
        perror("Sender: setsockopt IPPROTO_IP IP_MULTICAST_IF");
        exit(EXIT_FAILURE);
    }



    // // sending a message to client/multicast
    const char *msg = "Hello from sender, anyone in this group should recerve this.";
    // sendto(client_sockets[i] , msg , strlen(msg) , 0 );
    /* Send a message to the multicast group specified by the group_address sockaddr-structure. */
    string msg_str = "Hello from sender (pid " + to_string(getpid()) + " ), anyone in this group should recerve this.";
    char msg_char_array[msg_str.length() + 1];
    strcpy(msg_char_array, msg_str.c_str());
    if(sendto(sender_socket_fd, &msg_char_array, strlen(msg_char_array), 0, (struct sockaddr*)&group_address, sizeof(group_address)) < 0)
    {
        perror("Sender: Sending datagram message error");
        exit(EXIT_FAILURE);
    }


    printf("Sender: server stopped. \n");
    close(sender_socket_fd);
    return 0;
}
