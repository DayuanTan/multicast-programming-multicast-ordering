#include "buffered_delivered_msg.h"

using namespace std;


// global variables
// All buffered msgs. Index of out layer vector is proc_no-1. The order of inner vector is received order.
vector<vector<s_Seq_Msg>> buffered_msgs;

// All delivered msgs. Index of out layer vector is proc_no-1. The order of inner vector is delivered order.
vector<vector<s_Seq_Msg>> delivered_msgs;


void print_buffered_msgs(int proc_no){
    cout << "\n All buffered msgs are:\n";
    for (int i = 0; i < buffered_msgs.at(proc_no).size(); i++){
        cout << buffered_msgs.at(proc_no).at(i).msg << endl;
    }
    cout << endl << endl << endl;
}

void buffer_msg(int proc_no, int sequence_no, string msg){
    s_Seq_Msg new_msg;
    new_msg.sequence = sequence_no;
    new_msg.msg = msg;
    buffered_msgs.at(proc_no-1).push_back(new_msg);

    print_buffered_msgs(proc_no);
}

void print_delivered_msgs(int proc_no){
    cout << "\n All delivered msgs are:\n";
    for (int i = 0; i < delivered_msgs.at(proc_no).size(); i++){
        cout << delivered_msgs.at(proc_no).at(i).msg << endl;
    }
    cout << endl << endl << endl;
}

void deliver_msg(int proc_no, int sequence_no, string msg){
    s_Seq_Msg new_msg;
    new_msg.sequence = sequence_no;
    new_msg.msg = msg;
    delivered_msgs.at(proc_no-1).push_back(new_msg);

    print_delivered_msgs(proc_no);
}