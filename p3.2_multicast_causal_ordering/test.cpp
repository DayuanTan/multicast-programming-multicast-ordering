#include <string.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
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


using namespace std;



struct s_Seq_Msg
{
    int sequence; // clock value, aka sequence number
    std::string msg; // the received msg string corresponding to this sequence number
};


int main(){
    std::vector<std::vector<s_Seq_Msg>> buffered_msgs;

    for (int i =0; i < 3; i++){
        buffered_msgs.push_back(vector<s_Seq_Msg>());
    }

    cout << buffered_msgs.size() << endl;

    for (int i =0; i < 3; i++){
        cout << buffered_msgs.at(i).size() <<  endl;
    }

    
    struct s_Seq_Msg new_msg;
    new_msg.sequence = 1;
    new_msg.msg = "1111";
    buffered_msgs.at(0).push_back(new_msg);

    cout << buffered_msgs.at(0).at(0).sequence << endl;

    //for (int i =0; i < 3; i++){
        //int i =0;
        //cout << buffered_msgs.at(i).size() << buffered_msgs.at(i).sequence << buffered_msgs.at(i).msg <<  endl;
    //}

}