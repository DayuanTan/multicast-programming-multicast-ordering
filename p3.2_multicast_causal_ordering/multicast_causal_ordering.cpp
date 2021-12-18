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
#include <thread>
#include <bits/stdc++.h>

#include "helper.h"  
#include "buffered_delivered_msg.h"

#define PORT 5555

using namespace std;


// global variables

//NOTE: diff machine diff WIFI give you diff local ip addr. Use 'ifconfig' to check
const char LOCAL_IP_ADDR[] = "192.168.169.156";
//const cahr LOCAL_IP_ADDR[] = "192.168.169.154";




// argument vector<int>& pass vector by reference
void sender(int multicast_socket_fd, vector<int>& vector_clocks, int curr_proc_no)
{
    std::thread::id curr_threadID = std::this_thread::get_id();
    cout << "Sender starts. Sender pid is " <<  getpid() << " thread id is " << curr_threadID << endl;
    stringstream ss_tid;
    ss_tid << curr_threadID;
    string curr_threadID_str = ss_tid.str();

    cout << "Current vector_clocks is:" ;
    print_vecotr_clocks(vector_clocks);

    // *******
    // sender socket configuration 
    // *******


    /* Initialize the multicast group sockaddr structure with a */
    /* group address of 226.1.1.1 and port 5555. */
    struct sockaddr_in group_address;
    group_address.sin_family = AF_INET;    // IPv4
    group_address.sin_addr.s_addr = inet_addr("226.1.1.1"); //  = INADDR_ANY is localhost
    group_address.sin_port = htons( PORT ); // 5555


    // For multicast, the level (2nd param) should be IPPROTO_IP which has multiple options. 
    // IP_MULTICAST_LOOP: 0 if you want the data you send to be looped back to your host, 1 if not. Default is 0. We don't need to change.
    // IP_MULTICAST_TTL: Sets the Time To Live (TTL) in the IP header for outgoing multicast datagrams. 0 samehost. 1 same subnet. Default is 1. 
    u_char ttl = '0'; // because we use only one machine 
    if (setsockopt(multicast_socket_fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0)                                              
    {
        perror("Sender: setsockopt IPPROTO_IP IP_MULTICAST_TTL");
        exit(EXIT_FAILURE);
    }
    /* Set local interface for outbound multicast datagrams. */
    /* The IP address specified must be associated with a local multicast capable interface. */
    struct in_addr localInterface;
    localInterface.s_addr = inet_addr(LOCAL_IP_ADDR); // On linux use ifconfig to check "UP ... MULTICAST"
    if(setsockopt(multicast_socket_fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0)
    {
        perror("Sender: setsockopt IPPROTO_IP IP_MULTICAST_IF");
        exit(EXIT_FAILURE);
    }


    for(int i = 0; i < 10; i++){
        // increaes self clock by 1 before sending
        vector_clocks.at(curr_proc_no-1) ++;

        // // sending a message to client/multicast
        // const char *msg = "Hello from sender, anyone in this group should recerve this.";
        // sendto(client_sockets[i] , msg , strlen(msg) , 0 );
        /* Send a message to the multicast group specified by the group_address sockaddr-structure. */ 
        // FIFO: the number before first 2 "," are essential  
        string msg_str = to_string(vector_clocks.at(curr_proc_no-1)) + ", " + to_string(curr_proc_no) + ", are my local clock value and my proc_no. Msg No.=" + to_string(i) + ". From sender (ProcessNode No.= " + to_string(curr_proc_no) + " pid " + to_string(getpid()) + " threadid " + curr_threadID_str + "), anyone in this group should recerve this.";
        char msg_char_array[msg_str.length() + 1];
        strcpy(msg_char_array, msg_str.c_str());
        if(sendto(multicast_socket_fd, &msg_char_array, strlen(msg_char_array), 0, (struct sockaddr*)&group_address, sizeof(group_address)) < 0)
        {
            perror("Sender: Sending datagram message error");
            exit(EXIT_FAILURE);
        }

        cout << "------\nMessge sent, so vector_clocks updated: "; 
        print_vecotr_clocks(vector_clocks);
        usleep( 2 * 1000000 ); //microseconds 10^6 = 1 second
    }
}



void receiver(int multicast_socket_fd, vector<int>& vector_clocks, int curr_proc_no)
{
    std::thread::id curr_threadID = std::this_thread::get_id();
    cout << "Receiver starts. Receiver pid is " <<  getpid() << " thread id is " << curr_threadID << endl;
    cout << "Current vector_clocks is:" ;
    print_vecotr_clocks(vector_clocks);



    /* Bind to the proper port number with the IP address specified as INADDR_ANY. */
    struct sockaddr_in receiver_addr;
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr = INADDR_ANY; //is localhost
    receiver_addr.sin_port = htons(PORT); 
    socklen_t receiver_addr_length = sizeof(receiver_addr);
    // Forcefully attaching socket to the port 8080
    if (bind(multicast_socket_fd, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr)) < 0)
    {
        perror("Receiver: bind failed");
        exit(EXIT_FAILURE);
    } 
    

    /* Join the multicast group 226.1.1.1 on the local interface LOCAL_IP_ADDR interface. */
    /* Note that this IP_ADD_MEMBERSHIP option must be */
    /* called for each local interface over which the multicast */
    /* datagrams are to be received. */
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
    group.imr_interface.s_addr = inet_addr(LOCAL_IP_ADDR);
    if (setsockopt(multicast_socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
    {
        perror("Receiver: Adding multicast group error");
        exit(EXIT_FAILURE);
    }
    

    char receiver_read_buffer[1024] = {0}; // compile will be slow if this is too large
    while(1){
        // receiving form multicast
        // read() and recvfrom() are same here
        // if (read(multicast_socket_fd , receiver_read_buffer, 1024) < 0){
        if (recvfrom(multicast_socket_fd, receiver_read_buffer, 1024, 0, (struct sockaddr *)&receiver_addr, &receiver_addr_length) < 0){
            perror("Receiver: recvfrom() error");
            exit(EXIT_FAILURE);
        }
        printf("------\nReceiver: read: '%s'\n\n",receiver_read_buffer );


        // update vector_clocks according to received message
        // Only first curr_proc_no elements are needed
        string received_msg_str = receiver_read_buffer;
        vector<string> splited = split( received_msg_str, ",");
        cout << "Splited result is : " << vectorstr2str(splited) << endl;
        int recv_clock_value = stoi(splited[0]);// aka sequence no
        int recv_proc_no = stoi(splited[1]);

        // no action if the msg come from self
        if (recv_proc_no == curr_proc_no){
            cout << "\nMessage came from self, no action needed.\n\n";
            continue;
        }


        if (splited.size() >= 2){ // bc for FIFO only first 2 are essential

            if (int(vector_clocks.at(recv_proc_no-1) + 1) == recv_clock_value){
                // deliver msg
                deliver_msg(recv_proc_no, recv_clock_value, received_msg_str);
                vector_clocks.at(recv_proc_no-1) = recv_clock_value; 
                
                // check buffered msg which meet requirement
                check_buffered_msgs_and_deliver(recv_proc_no, recv_clock_value, vector_clocks);

                cout << "Vector_clocks updated: ";
                print_vecotr_clocks(vector_clocks);
            }else{
                // buffer msg
                buffer_msg(recv_proc_no, recv_clock_value, received_msg_str);
                cout << "Vector_clocks didn't update: ";
                print_vecotr_clocks(vector_clocks);
            }

            
        }else{
            perror("Receiver: vector_clocks size of received message is not enough.");
            exit(EXIT_FAILURE);
        }
        

        memset(receiver_read_buffer, '\0', strlen(receiver_read_buffer));
    }
    
    

}


 
int main(int argc, char *argv[])
{
    // /* deal with input arguments*/
    // std::cout << "print arguments:\nargc == " << argc << '\n';
    // for(int ndx{}; ndx != argc; ++ndx) {
    //     std::cout << "argv[" << ndx << "] == " << argv[ndx] << '\n';
    // }
    // std::cout << "argv[" << argc << "] == "
    //           << static_cast<void*>(argv[argc]) << '\n';
    

    // *******
    // both need setting begin
    // *******
    printf("Multicast starts. The pid is %d \n", getpid());


    
    // Socket Cite: https://www.geeksforgeeks.org/socket-programming-cc/?ref=lbp
    // Multiocast Cite: https://www.tenouk.com/Module41c.html
    int multicast_socket_fd; 

    // // Creating socket file descriptor (IPv4, TCP, IP)
    // if ((sender_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    // For multicast it must be family AF_INET, and its type may be either SOCK_DGRAM or SOCK_RAW. The most common use is with SOCK_DGRAM sockets
    if ((multicast_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) // (IPv4, UDP, IP)
    {
        perror("Multicast: socket failed");
        exit(EXIT_FAILURE);
    }
       

    // Optional: it helps in reuse of address and port. Prevents error such as: “address already in use”.
    int opt = 1; // for setsockopt
    if (setsockopt(multicast_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
        perror("Multicast: setsockopt SOL_SOCKET");
        exit(EXIT_FAILURE);
    }
    // *******
    // both need setting end
    // *******






    // *******
    // Causal ordering begin
    // *******

    // ask user input (also clean processes_counter if this is first created proc)
    int declared_proc_amount = ask_user_input_declared_proc_amt();
    // write into file
    if (declare_processes_amount(declared_proc_amount) <= 0 ){
        perror("Multicast: Declared proc amount write into file error.");
        exit(EXIT_FAILURE);
    }


    int proc_ctr_current_value = processes_counter(1); // increase process counter by 1
    cout << "\nThere are " << proc_ctr_current_value << " processes/nodes in this DS now.\n" << endl;
    // Register signal and signal handler
    signal(SIGINT, signal_callback_handler);// wait for Ctrol+c, if caught then terminate current process

    
    // hangon here if not enough process created
    int curr_proc_amount = processes_counter(0);
    while (declared_proc_amount != curr_proc_amount){
        curr_proc_amount = processes_counter(0);
    }


    int curr_proc_no = proc_ctr_current_value; // the no. of current process in all processes, e.g. it is 3 for third process. 
    cout << "Current process(node) is No. " << curr_proc_no << endl << endl;
    for (int i =0; i < declared_proc_amount; i++){
        vector_clocks.push_back(0);
        buffered_msgs.push_back(vector<s_Seq_Msg>());
        delivered_msgs.push_back(vector<s_Seq_Msg>());
    }


    // *******
    // Causal ordering end
    // *******





    
    
    
    // This thread is launched by using function pointer as callable.
    // The parameters to the function are put after the comma
    std::thread thread_send_obj(sender, multicast_socket_fd, std::ref(vector_clocks), curr_proc_no);
    std::thread thread_receive_obj(receiver, multicast_socket_fd, std::ref(vector_clocks), curr_proc_no);// must be send first otherwise bind() error
    // Wait for the threads to finish. main() and these two threads are concurrent/synchronized.
    thread_send_obj.join();
    thread_receive_obj.join();
    
    

    

    
    proc_ctr_current_value = processes_counter(-1);  // decrease counter by 1
    printf("Multicast: process stopped. \n");
    cout << "There are still " << proc_ctr_current_value << " processes/nodes left in this DS now." << endl;

    close(multicast_socket_fd);
    return 0;
}
