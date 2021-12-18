#ifndef BUFFERED_DELIVERED_MSG    
#define BUFFERED_DELIVERED_MSG

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <vector>
#include <bits/stdc++.h>
#include <sstream>



// not place a "using namespace std" directive in the C++ header file because it may cause silent name clashes between different libraries. To remedy this, use the fully qualified name:  std::string

struct s_Seq_Msg
{
    int sequence; // clock value, aka sequence number
    std::string msg; // the received msg string corresponding to this sequence number
};



void print_buffered_msgs(int proc_no);

void buffer_msg(int proc_no, int sequence_no, std::string msg);

void print_delivered_msgs(int proc_no);

void deliver_msg(int proc_no, int sequence_no, std::string msg);

#endif