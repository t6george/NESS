#include <iostream>
#include <cassert>
#include <string>
#include <fstream>

#include "./cpu.cpp";

using namespace std;


int main (int argc, char* argv[]) {
    assert(argc > 1);
    string filepath (argv[1]);

    ifstream ROM (filepath, ios::binary | ios::in);
    unsigned short byte;
    unsigned char b;

    CPU cpu = new CPU();

    if (ROM.is_open()) {
      while (ROM >> b) {
        byte = b;

        //cout << byte << endl;
      }
      ROM.close();
    } else {
      cerr << "Unable to open file in location " << filepath << endl;
    }

    return 0;

}
