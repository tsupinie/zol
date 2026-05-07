
#include <vector>
#include <sstream>

#include "nexrad_l2_message.h"

#ifndef __NEXRAD_L2_CHUNK__
#define __NEXRAD_L2_CHUNK__

class NexradL2Chunk {
    public:
        NexradL2Chunk(std::vector<NexradL2Message*>);
        ~NexradL2Chunk();

        static NexradL2Chunk* from_binary(std::istream&, char);

        std::vector<float> get_volume_elevation_angles() const;

    private:
        std::vector<NexradL2Message*> messages;
};

#endif