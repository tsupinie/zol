
#include <string>
#include <cstdint>
#include <iostream>

#ifndef __NEXRAD_L2_HEADER__
#define __NEXRAD_L2_HEADER__

class NexradL2MessageHeader {
    public:
        friend std::istream& operator>>(std::istream&, NexradL2MessageHeader&);

        uint8_t get_message_type() const { return this->msg_type; }
        uint16_t get_message_size() const { return 2 * this->msg_size; }

        std::string dump();

    private:
        uint16_t msg_size;
        uint8_t rda_channel;
        uint8_t msg_type;

        uint16_t msg_sequence_num;
        uint16_t julian_day;
        
        uint32_t ms_since_midnight;

        uint16_t number_of_segs;
        uint16_t seg_num;
};

#endif