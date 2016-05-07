#include <iostream>
using namespace std;

#include "gflags/gflags.h"


DEFINE_string(id, "", "module id");

int main(int argc, char* argv[])
{
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    cout << FLAGS_id << endl;
    return 0; 
}
