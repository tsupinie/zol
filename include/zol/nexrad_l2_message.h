
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <memory>

#include "nexrad_l2_header.h"

#ifndef __NEXRAD_L2_MESSAGE__
#define __NEXRAD_L2_MESSAGE__

class NexradL2Message {
    public:
        virtual ~NexradL2Message() {};
        static std::unique_ptr<NexradL2Message> factory(std::istream&);
        virtual std::string dump();

        virtual bool is_blank() const { return false; }
        friend std::istream& operator>>(std::istream&, NexradL2Message&);

        uint8_t get_message_type() const { return this->header->get_message_type(); }

    protected:
        std::unique_ptr<NexradL2MessageHeader> header;
};

template <uint8_t N>
class NexradL2MessageType : public NexradL2Message {
    public:
        static constexpr uint8_t type = N;
};

class NexradL2Message0: public NexradL2MessageType<0> {
    public:
        bool is_blank() const { return true; }
};

#endif