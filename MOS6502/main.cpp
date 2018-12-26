#include <iostream>
#include <cassert>
#include <string>

#include "../Headers/cpu.hpp"

using namespace std;


int main (int argc, char* argv[]) {
    assert (argc > 1);
    string filepath (argv[1]);

    CPU * cpu = new CPU ();
    cpu->initCartridge (filepath);
    cpu->printPrgRom ();

    return 0;

}
