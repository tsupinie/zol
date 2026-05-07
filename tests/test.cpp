
#include <iostream>
#include <fstream>
#include <string>

#include <nexrad_l2_chunk.h>

int main(int argc, char** argv) {
    std::ifstream ifile;
    std::string fname = "/Users/tsupinie/data/level2/KDGX_20260506_005539_chunks/20260507-005539-106-E";

    ifile.open(fname);

    NexradL2Chunk* l2c;

    try {
        l2c = NexradL2Chunk::from_binary(ifile, fname.back());
    }
    catch (std::string exc) {
        std::cerr << exc << std::endl;
        return 1;
    }

    return 0;
}