#include <iostream>
#include <cassert>
#include <string>
#include <fstream>

#include "../Headers/cpu.hpp"

using namespace std;


int main (int argc, char* argv[]) {
    assert(argc > 1);
    string filepath (argv[1]);

    ifstream ROM (filepath, ios::binary | ios::in);
    char b;
    int i = 0;

    if (ROM.is_open()) {
      while (i < 5) {
        ROM.read(&b, INSTR_SIZE);
        cout << b;
        i++;
      }
      ROM.close();
    } else {
      cerr << "Unable to open file in location " << filepath << endl;
    }

    return 0;

}
