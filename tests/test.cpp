
#include <iostream>
#include <fstream>
#include <string>

#include <nexrad_l2_chunk.h>

void test_start_chunk() {
    std::ifstream ifile;
    std::string fname = "data/20260507-005539-001-S";

    ifile.open(fname);

    NexradL2VolumeHeaderChunk l2c;

    try {
        l2c = NexradL2VolumeHeaderChunk::from_binary(ifile);
    }
    catch (std::string exc) {
        std::cerr << "Error: " << exc << std::endl;
        return;
    }

    std::vector<float> data = l2c.get_volume_elevation_angles();
    for (size_t i = 0 ; i < data.size(); i++) {
        std::cout << data[i] << ", ";
    }
    std::cout << std::endl;
}

void test_intermediate_chunk() {
    std::ifstream ifile;
    std::string fname = "data/20260507-005539-002-I";

    ifile.open(fname);

    NexradL2VolumeChunk l2c;

    try {
        l2c = NexradL2VolumeChunk::from_binary(ifile);
    }
    catch (std::string exc) {
        std::cerr << "Error: " << exc << std::endl;
        return;
    }

    std::vector<float> data = l2c.get_moment_data("ZDR");
    for (size_t i = 0 ; i < 10; i++) {
        std::cout << data[i] << ", ";
    }
    std::cout << std::endl;
}

void test_end_chunk() {
    std::ifstream ifile;
    std::string fname = "data/20260507-005539-106-E";

    ifile.open(fname);

    NexradL2VolumeChunk l2c;

    try {
        l2c = NexradL2VolumeChunk::from_binary(ifile);
    }
    catch (std::string exc) {
        std::cerr << "Error: " << exc << std::endl;
        return;
    }

    std::vector<float> data = l2c.get_moment_data("ZDR");
    for (size_t i = 0 ; i < 10; i++) {
        std::cout << data[i] << ", ";
    }
    std::cout << std::endl;   
}

int main(int argc, char** argv) {
    test_start_chunk();
    test_intermediate_chunk();
    test_end_chunk();
}