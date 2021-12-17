
#include "helper.h"  


// operation arguemnt: 0 - fetech curr proc count; 1 increase counter by 1; -1 decrease counter by 1
int processes_counter(int operation){
    int proc_ctr_current_value; 
    string line;
    ifstream proc_ctr_file;

    proc_ctr_file.open(PROCESSES_COUNTER_FILE_NAME);

    if (proc_ctr_file.is_open()){
        try{
            getline(proc_ctr_file, line);
            proc_ctr_current_value = stoi(line.c_str());
            if (operation == 0){ // only read current value
                proc_ctr_file.close();
                return proc_ctr_current_value;
            }
        }catch(std::exception const & e) {
            cout << "Exception Caught '" << e.what() << "'.\n"; 
        }
        
        proc_ctr_file.close();
    } else {
        cout << "Unable to open the '" << PROCESSES_COUNTER_FILE_NAME << "' file, please check the name!\n"; 
        exit(-1);
    }

    // increase or decrease the processes counter value
    ofstream proc_ctr_file_modify;
    proc_ctr_file_modify.open(PROCESSES_COUNTER_FILE_NAME, ios::trunc); // ios::trunc: previous content is deleted
    if (proc_ctr_file_modify.is_open()){
        if (operation == 1){// increase by 1
            proc_ctr_current_value ++;
        }else if (operation == -1){// decrease by 1
            proc_ctr_current_value --;
        }

        // write into 
        proc_ctr_file_modify << proc_ctr_current_value;
        proc_ctr_file_modify.close();
    } else {
        cout << "Unable to open the '" << PROCESSES_COUNTER_FILE_NAME << "' file, please check the name!\n"; 
        exit(-1);
    }

    return proc_ctr_current_value;
}




int declare_processes_amount(int declared_amount){
    ofstream proc_amount_declared;
    proc_amount_declared.open(PROCESSES_AMOUNT_DECLARED_FILE_NAME, ios::trunc); // ios::trunc: previous content is deleted
    if (proc_amount_declared.is_open()){
        // write into 
        proc_amount_declared << declared_amount;
        proc_amount_declared.close();
    } else {
        cout << "Unable to open the '" << PROCESSES_AMOUNT_DECLARED_FILE_NAME << "' file, please check the name!\n"; 
        exit(-1);
    }

    return declared_amount;
}

int fetch_processes_declared_amount(){
    string line;
    ifstream proc_amount_declared;
    proc_amount_declared.open(PROCESSES_AMOUNT_DECLARED_FILE_NAME);

    if (proc_amount_declared.is_open()){
        try{
            getline(proc_amount_declared, line);
            int proc_ctr_current_value = stoi(line.c_str());
            proc_amount_declared.close();
            return proc_ctr_current_value;
        }catch(std::exception const & e) {
            cout << "Exception Caught '" << e.what() << "'.\n"; 
        }
    } else {
        cout << "Unable to open the '" << PROCESSES_AMOUNT_DECLARED_FILE_NAME << "' file, please check the name!\n"; 
        exit(-1);
    }
}

int ask_user_input_declared_proc_amt(){
    int declared_amount = fetch_processes_declared_amount();
    if (declared_amount <= 2) {
        cout << "\nHow many processes(nodes) are you going to create (which must >= 3 and you must follow)? Please input a positive integer: ";
        try{
            cin >> declared_amount;
        }catch(std::exception const & e) {
            cout << "Invalid input. Exception Caught '" << e.what() << "'.\n"; 
        }
        cout << "\nYou declared you will create " << declared_amount << " processes(nodes). You must follow it.\n" << endl;
    
        // bc this must be first process so also clean processes_counter to double ensure
        // This avoid the effect of sb closed terminal windows directly at last running
        int proc_ctr = processes_counter(0);
        if(proc_ctr != 0){
            ofstream proc_ctr_cleaner;
            proc_ctr_cleaner.open(PROCESSES_COUNTER_FILE_NAME, ios::trunc); // ios::trunc: previous content is deleted
            if (proc_ctr_cleaner.is_open()){
                // write into 
                proc_ctr_cleaner << 0;
                proc_ctr_cleaner.close();
            } else {
                cout << "Unable to open the '" << PROCESSES_COUNTER_FILE_NAME << "' file, please check the name!\n"; 
                exit(-1);
            }
        }
    }
    return declared_amount;
}

string vectorint2str(vector<int>& vc){
    std::stringstream ss;
    for (int i = 0; i < vc.size()-1; i++) {
        ss << vc.at(i) << ", ";
    }
    ss << vc.back();
    std::string s = ss.str();
    return s;
}

string vectorstr2str(vector<string>& vc){
    std::stringstream ss;
    for (int i = 0; i < vc.size()-1; i++) {
        ss << vc.at(i) << ", ";
    }
    ss << vc.back();
    std::string s = ss.str();
    return s;
}


void print_vecotr_clocks(vector<int>& vc){
    // Print out the vector
    std::cout << "\nvector_clocks = {";
    std::cout << vectorint2str(vc) << "}; \n\n";
}


// function for string delimiter
vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back( token );
    }

    res.push_back( s.substr(pos_start) );
    return res;
}


// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
    cout << "\n\nCaught Ctrl+c signal  " << signum << endl;

    int proc_ctr_current_value = processes_counter(-1);  // decrease counter by 1
    declare_processes_amount(0); // clean the declared proc amount
    printf("Multicast: process stopped. \n");
    cout << "There are still " << proc_ctr_current_value << " processes/nodes left in this DS now." << endl;


    // Terminate program
    exit(signum);
}