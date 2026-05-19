
#include <vector>
#include <sstream>
#include <memory>
#include <chrono>

#include "nexrad_l2_message.h"

#ifndef __NEXRAD_L2_CHUNK__
#define __NEXRAD_L2_CHUNK__


class NexradL2VolumeHeaderChunk {
    public:
        NexradL2VolumeHeaderChunk() = default;
        NexradL2VolumeHeaderChunk(std::vector<std::unique_ptr<NexradL2Message>> messages) : messages(std::move(messages)) {};
        static NexradL2VolumeHeaderChunk from_binary(std::istream& istream);

        std::vector<float> get_volume_elevation_angles() const;
    private:
        std::vector<std::unique_ptr<NexradL2Message>> messages {};

        template <class T>
        T* get_first_message() const;
};

class NexradL2VolumeChunk {
    public:
        NexradL2VolumeChunk() = default;
        NexradL2VolumeChunk(std::vector<std::unique_ptr<NexradL2Message>> messages) : messages(std::move(messages)) {};
        static NexradL2VolumeChunk from_binary(std::istream& istream);

        std::vector<std::string> get_moments() const;
        std::vector<float> get_moment_data(const std::string&) const;
        std::vector<float> get_radial_angles_from_north() const;
        std::vector<std::chrono::time_point<std::chrono::system_clock>> get_radial_times() const;
    private:
        std::vector<std::unique_ptr<NexradL2Message>> messages {};

        template <class T, typename F>
        void for_each_message(F&&) const;

        template <class T>
        T* get_first_message() const;
};

#endif