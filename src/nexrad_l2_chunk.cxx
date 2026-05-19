
#include <string>
#include <fstream>
#include <iostream>

#include <bzlib.h>

#include <nexrad_l2_chunk.h>
#include <nexrad_l2_message5.h>
#include <nexrad_l2_message31.h>
#include <utils.h>


void decompress_bz2(std::istream& instream, std::stringstream& outstream) {
    const size_t bufinsize = 256;
    char buffer_in[bufinsize] = { 0 };
    const size_t bufoutsize = 256;
    char buffer_out[bufoutsize] = { 0 };

    bz_stream strm = {0};
    int bzstat;
    bzstat = BZ2_bzDecompressInit(&strm, 0, 0);
    if (bzstat != BZ_OK) {
        throw "DecompressInit failed with code " + std::to_string(bzstat);
    }

    long int total_in, total_out;

    do {
        strm.next_in = buffer_in;
        strm.next_out = buffer_out;

        instream.read(buffer_in, bufinsize);
        strm.avail_in = instream.gcount();
        strm.avail_out = bufoutsize;

        bzstat = BZ2_bzDecompress(&strm);
/*
        total_in = ((long int)(strm.total_in_hi32) << 32) + strm.total_in_lo32;
        total_out = ((long int)(strm.total_out_hi32) << 32) + strm.total_out_lo32;

        Logging::debug << "Supplied " + std::to_string(instream.gcount()) + " bytes to bzip2 (" + std::to_string(total_in) + " in total) and got " 
                          + std::to_string(bufoutsize - strm.avail_out) + " bytes out (" + std::to_string(total_out) + " in total)";
*/
        if (bzstat != BZ_OK) {
            throw "Decompress returned code " + std::to_string(bzstat);
        }

        outstream.write(buffer_out, bufoutsize - strm.avail_out);

    } while (bzstat == BZ_OK && (instream.gcount() > 0 || strm.avail_out == 0));

    bzstat = BZ2_bzDecompressEnd(&strm);
    if (bzstat != BZ_OK) {
        throw "DecompressEnd failed with code " + std::to_string(bzstat);
    }
}

std::vector<std::unique_ptr<NexradL2Message>> message_list_from_stream(std::istream& instream, bool should_be_start_chunk) {
    std::stringstream decompressed;

    std::string tape_filename, radar_id;
    uint32_t volume_julian_date, volume_ms_since_midnight;
    int32_t rec_size;

    size_t start_ptr = instream.tellg();
    const bool is_start_chunk = READ(instream, 4) == "AR2V";
    instream.seekg(start_ptr);

    if (should_be_start_chunk != is_start_chunk) {
        if (is_start_chunk) throw std::string("It looks like this is a header chunk, but a volume chunk was requested");
        else throw std::string("It looks like this is a volume chunk, but a header chunk was requested");
    }

    if (should_be_start_chunk) {
        tape_filename = READ(instream, 12);
        volume_julian_date = READ<uint32_t>(instream);
        volume_ms_since_midnight = READ<uint32_t>(instream);
        radar_id = READ(instream, 4);
    }

    rec_size = READ<int32_t>(instream);

    decompress_bz2(instream, decompressed);

    uint32_t network_msg_type, network_msg_type_dep, network_data_size;
    std::vector<std::unique_ptr<NexradL2Message>> messages;

    int cur_pos = decompressed.tellg();
    decompressed.seekg(0, std::ios_base::end);
    int end = decompressed.tellg();

    decompressed.seekg(cur_pos);

    while (cur_pos != end && cur_pos != -1) {
        READ<uint32_t>(decompressed);
        READ<uint32_t>(decompressed);
        READ<uint32_t>(decompressed);

        std::unique_ptr<NexradL2Message> msg = NexradL2Message::factory(decompressed);

        if (!msg->is_blank()) {
            messages.push_back(std::move(msg));
        }

        cur_pos = decompressed.tellg();
    }

    return messages;
}

NexradL2VolumeHeaderChunk NexradL2VolumeHeaderChunk::from_binary(std::istream& instream) {
    return NexradL2VolumeHeaderChunk(message_list_from_stream(instream, true));
}

std::vector<float> NexradL2VolumeHeaderChunk::get_volume_elevation_angles() const {
    NexradL2Message* msg = NULL;
    for (auto&& msg_ptr : this->messages) {
        if (msg_ptr->get_message_type() == 5) {
            msg = msg_ptr.get();
        }
    }

    if (msg == NULL) {
        throw "No Message 5 in this chunk";
    }

    return static_cast<NexradL2Message5*>(msg)->get_elevation_angles();
}

NexradL2VolumeChunk NexradL2VolumeChunk::from_binary(std::istream& instream) {
    return NexradL2VolumeChunk(message_list_from_stream(instream, false));
}

std::vector<std::string> NexradL2VolumeChunk::get_moments() const {
    // This function assumes the the set of moments is fixed across all radials in a chunk. I think this is a good assumption, but maybe not.
    for (auto&& msg_ptr : this->messages) {
        if (msg_ptr->get_message_type() == 31) {
            return static_cast<NexradL2Message31*>(msg_ptr.get())->get_moments();
        }
    }

    throw std::string("No Message 31 in this chunk (this shouldn't happen)");
}

std::vector<float> NexradL2VolumeChunk::get_moment_data(const std::string& moment) const {
    size_t n_gates = 0;

    this->for_each_message_31([&n_gates](const NexradL2Message31& msg) {
        n_gates += msg.get_n_gates();
    });

    std::vector<float> data(n_gates);
    size_t idata = 0;

    this->for_each_message_31([&data, &idata, moment](const NexradL2Message31& msg) {
        for (float dat : msg.get_moment_data(moment)) {
            data[idata++] = dat;
        }
    });

    return data;
}

std::vector<float> NexradL2VolumeChunk::get_radial_angles_from_north() const {
    std::vector<float> bearings;

    this->for_each_message_31([&bearings](const NexradL2Message31& msg) {
        bearings.push_back(msg.get_radial_angle_from_north());
    });

    return bearings;
}

std::vector<std::chrono::time_point<std::chrono::system_clock>> NexradL2VolumeChunk::get_radial_times() const {
    std::vector<std::chrono::time_point<std::chrono::system_clock>> times;

    this->for_each_message_31([&times](const NexradL2Message31& msg) {
        times.push_back(msg.get_radial_time());
    });

    return times;
}

template <typename F>
void NexradL2VolumeChunk::for_each_message_31(F&& func) const {
    for (auto&& msg_ptr : this->messages) {
        if (msg_ptr->get_message_type() == 31) {
            func(*static_cast<NexradL2Message31*>(msg_ptr.get()));
        }
    }
}