#include "general_test_functions.h"

using namespace std;

Record_Output::Record_Output() {
  silent = false;
  old_cout = cout.rdbuf();
  old_cerr = cerr.rdbuf();
}

void Record_Output::start_recording_cout() {
  if(!silent) {
    cerr << "STARTING TO RECORD" << endl;
    cout.rdbuf(cout_buffer.rdbuf());
  } 
    cerr << "STARTING T2RECORD" << endl;
  cout_buffer.str("");
}

void Record_Output::stop_recording_cout() {
  if(!silent) {
    cout.rdbuf(old_cout);
  }
  cout_output = cout_buffer.str();
}

string Record_Output:: get_cout_output() {
  return cout_output;
}

void Record_Output::start_recording_cerr() {
  if(!silent) {
    cerr.rdbuf(cerr_buffer.rdbuf());
  }
  cerr_buffer.str("");
}

void Record_Output::stop_recording_cerr() {
  if(!silent) {
    cerr.rdbuf(old_cerr);
  }
  cerr_output = cerr_buffer.str();
}

string Record_Output:: get_cerr_output() {
  return cerr_output;
}

void Record_Output::silence() {
  if(!silent) {
    silent = true;
    cout.rdbuf(cout_buffer.rdbuf());
    cerr.rdbuf(cerr_buffer.rdbuf());
  }
}

void Record_Output::unsilence() {
  if(silent) {
    silent = false;

    cerr.rdbuf(old_cerr);
    cerr_buffer.str("");

    cout.rdbuf(old_cout);
    cout_buffer.str("");
  }
}


