
#include <string>
#include <fstream>

#include <bzlib.h>

#include <nexrad_l2_chunk.h>
#include <nexrad_l2_message5.h>
#include <bin_utils.h>


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

NexradL2Chunk::NexradL2Chunk(std::vector<NexradL2Message*> messages) {
    this->messages = messages;
}

NexradL2Chunk::~NexradL2Chunk() {

}

NexradL2Chunk* NexradL2Chunk::from_binary(std::istream& instream, char chunk_type) {
    std::stringstream decompressed;

    std::string tape_filename, radar_id;
    uint32_t volume_julian_date, volume_ms_since_midnight;
    int32_t rec_size;

    if (chunk_type == 'S') {
        tape_filename = READ(instream, 12);
        volume_julian_date = READ<uint32_t>(instream);
        volume_ms_since_midnight = READ<uint32_t>(instream);
        radar_id = READ(instream, 4);
    }

    rec_size = READ<int32_t>(instream);

    decompress_bz2(instream, decompressed);

    uint32_t network_msg_type, network_msg_type_dep, network_data_size;
    std::vector<NexradL2Message*> messages;

    int cur_pos = decompressed.tellg();
    decompressed.seekg(0, std::ios_base::end);
    int end = decompressed.tellg();

    decompressed.seekg(cur_pos);

    while (cur_pos != end && cur_pos != -1) {
        READ<uint32_t>(decompressed);
        READ<uint32_t>(decompressed);
        READ<uint32_t>(decompressed);

        NexradL2Message* msg = NexradL2Message::factory(decompressed);

        if (!msg->is_blank()) {
            messages.push_back(msg);
        }
        else {
            delete msg;
        }

        cur_pos = decompressed.tellg();
    }

    return new NexradL2Chunk(messages);
}

std::vector<float> NexradL2Chunk::get_volume_elevation_angles() const {
    NexradL2Message* msg = NULL;
    for (NexradL2Message* msg_ : this->messages) {
        if (msg_->get_message_type() == 5) {
            msg = msg_;
        }
    }

    if (msg == NULL) {
        throw "No Message 5 in this chunk";
    }

    return static_cast<NexradL2Message5*>(msg)->get_elevation_angles();
}