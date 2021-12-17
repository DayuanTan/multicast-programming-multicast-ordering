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

using namespace std;


const char PROCESSES_COUNTER_FILE_NAME[] = "processes_counter.txt"; 

const char PROCESSES_AMOUNT_DECLARED_FILE_NAME[] = "processes_amount_declared.txt"; 


int processes_counter(int operation);

int declare_processes_amount(int declared_amount);

int fetch_processes_declared_amount();

int ask_user_input_declared_proc_amt();

string vectorint2str(vector<int>& vc);

string vectorstr2str(vector<string>& vc);

void print_vecotr_clocks(vector<int>& vc);

vector<string> split(string s, string delimiter); 

void signal_callback_handler(int signum);

#endif