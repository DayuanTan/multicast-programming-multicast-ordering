#include "buffered_delivered_msg.h"
#include "helper.h"

using namespace std;

// global variables

// All buffered msgs. Index of out layer vector is proc_no-1. The order of inner vector is received order.
std::vector<std::vector<s_Seq_Msg>> buffered_msgs;

// All delivered msgs. Index of out layer vector is proc_no-1. The order of inner vector is delivered order.
std::vector<std::vector<s_Seq_Msg>> delivered_msgs;




void print_buffered_msgs(){
    int proc_declared_amount = fetch_processes_declared_amount();
    cout << "\n All buffered msgs are:\n";
    for (int proc_i = 0; proc_i < proc_declared_amount; proc_i++){
        for (int i = 0; i < buffered_msgs.at(proc_i).size(); i++){
            cout << buffered_msgs.at(proc_i).at(i).msg << endl;
        }
    }
    cout << endl << endl << endl;
}

void buffer_msg(int proc_no, int sequence_no, string msg){
    struct s_Seq_Msg new_msg;
    new_msg.sequence = sequence_no;
    new_msg.msg = msg;
    buffered_msgs.at(proc_no-1).push_back(new_msg);

    cout << "\nMessage buffered!\n";
    print_buffered_msgs();
}

void print_delivered_msgs(){
    int proc_declared_amount = fetch_processes_declared_amount();
    cout << "\n All delivered msgs are:\n";
    for (int proc_i = 0; proc_i < proc_declared_amount; proc_i++){
        for (int i = 0; i < delivered_msgs.at(proc_i).size(); i++){
            cout << delivered_msgs.at(proc_i).at(i).msg << endl;
        }
    }
    cout << endl << endl << endl;
}

void deliver_msg(int proc_no, int sequence_no, string msg){
    struct s_Seq_Msg new_msg;
    new_msg.sequence = sequence_no;
    new_msg.msg = msg;
    delivered_msgs.at(proc_no-1).push_back(new_msg);

    cout << "\nMessage delivered!\n";
    print_delivered_msgs();
}

int check_buffered_msgs_and_deliver(int curr_proc_no, int proc_no, int new_curr_clock_value, vector<int> & vector_clocks){
    stack<int> delivered_msg_index;
    int declared_proc_amount = fetch_processes_declared_amount();


    int originalsize = buffered_msgs.at(proc_no-1).size();
    for (int i=0; i < originalsize; i++){
        int seq_no = buffered_msgs.at(proc_no-1).at(i).sequence;
        if (seq_no == new_curr_clock_value+1){ // requirement1

            // requirement2
            bool meet_requirements = false;
            for (int j = 0; j < declared_proc_amount; j ++){
                if (j == proc_no-1 || j == curr_proc_no-1){
                    continue;// skip self, only check others
                }
                // requirement: local clock vector [i] >= received clock vector [i]
                if (vector_clocks.at(j) >= buffered_msgs.at(proc_no-1).at(j).sequence ){
                    meet_requirements = true;
                }else{
                    meet_requirements = false;
                    break;
                }
            }

            if (meet_requirements){
                // record for later delete from buffered_msgs
                delivered_msg_index.push(i);
                // deliver
                cout << "Check buffered msgs and deliver below msgs:\n";
                deliver_msg(proc_no, seq_no, buffered_msgs.at(proc_no-1).at(i).msg);
                // update local clock
                new_curr_clock_value ++;
                vector_clocks.at(proc_no-1) = new_curr_clock_value;
            }
        }
    }

    int ctr = delivered_msg_index.size();
    // delete from buffered_msgs
    while(!delivered_msg_index.empty()){
        buffered_msgs.at(proc_no-1).erase( buffered_msgs.at(proc_no-1).begin() + delivered_msg_index.top() );
        delivered_msg_index.pop(); //delete toppest element 
    }
    print_buffered_msgs();
    
    return ctr;
}