#ifndef HELPER_H    // To make sure don't declare the function more than once by including the header multiple times.
#define HELPER_H

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

// global
extern std::vector<int> vector_clocks; // vector clocks of knowledge of all processess/nodes


const char PROCESSES_COUNTER_FILE_NAME[] = "processes_counter.txt"; 

const char PROCESSES_AMOUNT_DECLARED_FILE_NAME[] = "processes_amount_declared.txt"; 


int processes_counter(int operation);

int declare_processes_amount(int declared_amount);

int fetch_processes_declared_amount();

int ask_user_input_declared_proc_amt();

std::string vectorint2str(std::vector<int>& vc);

std::string vectorstr2str(std::vector<std::string>& vc);

void print_vecotr_clocks(std::vector<int>& vc);

std::vector<std::string> split(std::string s, std::string delimiter); 

void signal_callback_handler(int signum);

#endif