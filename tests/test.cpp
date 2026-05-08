
#include <iostream>
#include <fstream>
#include <string>

#include <nexrad_l2_chunk.h>

int main(int argc, char** argv) {
    std::ifstream ifile;
    std::string fname = "/Users/tsupinie/data/level2/KDGX_20260506_005539_chunks/20260507-005539-002-I";

    ifile.open(fname);

    NexradL2VolumeChunk l2c;

    try {
        l2c = NexradL2VolumeChunk::from_binary(ifile);
    }
    catch (std::string exc) {
        std::cerr << exc << std::endl;
        return 1;
    }

    std::vector<float> data = l2c.get_moment_data("ZDR");
    for (size_t i = 0 ; i < 10; i++) {
        std::cout << data[i] << ", ";
    }
    std::cout << std::endl;

    return 0;
}