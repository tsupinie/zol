# zol
Read NEXRAD Level 2 data chunks

## Build
```bash
cmake -B build -S .
cmake --build build
```

## Use
```cpp
#include <fstream>
#include <chrono>

#include <zol.h>

std::ifstream ifile_header;
ifile_header.open("data/20260507-005539-001-S");

// Read header chunk from file
NexradL2VolumeHeaderChunk l2ch = NexradL2VolumeHeaderChunk::from_binary(ifile_header);

// Get the elevation angles for this volume
std::vector<float> data = l2ch.get_volume_elevation_angles();

std::ifstream ifile_vol;
ifile_vol.open("data/20260507-005539-002-I");

// Read intermediate or end chunk from file
NexradL2VolumeChunk l2cv = NexradL2VolumeChunk::from_binary(ifile_vol);

// Get list of moments in this chunk
std::vector<std::string> moments = l2cv.get_moments();

// Get data for one of the moments 
std::vector<float> data = l2cv.get_moment_data("ZDR");

// Get the radial angle as degrees clockwise from north
std::vector<float> bearings = l2cv.get_radial_angles_from_north();

// Get the times for each radial
std::vector<std::chrono::time_point<std::chrono::system_clock>> times = l2cv.get_radial_times();
```
