#include <iostream>
#include <cassert>
#include <string>

#include "./cpu.cpp";

using namespace std;


int main (int argc, char* argv[]) {
    assert(argc > 1);
    string filepath (argv[1]);

    CPU cpu = new CPU();


    return 0;

}
