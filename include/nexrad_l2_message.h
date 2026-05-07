
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "nexrad_l2_header.h"

#ifndef __NEXRAD_L2_MESSAGE__
#define __NEXRAD_L2_MESSAGE__

class NexradL2Message {
    public:
        NexradL2Message();
        virtual ~NexradL2Message();

        static NexradL2Message* factory(std::istream&);
        virtual std::string dump() = 0;

        virtual bool is_blank() const { return false; }

        uint8_t get_message_type() const { return this->header->get_message_type(); }

    protected:
        NexradL2MessageHeader* header;
};

class NexradL2MessageDummy : public NexradL2Message {
    public:
        std::string dump();
        friend std::istream& operator>>(std::istream&, NexradL2MessageDummy&);
};

class NexradL2Message0: public NexradL2MessageDummy {
    public:
        bool is_blank() const { return true; }
};

class NexradL2Message2: public NexradL2MessageDummy {};
class NexradL2Message3: public NexradL2MessageDummy {};
class NexradL2Message8: public NexradL2MessageDummy {};
class NexradL2Message13: public NexradL2MessageDummy {};
class NexradL2Message15: public NexradL2MessageDummy {};
class NexradL2Message18: public NexradL2MessageDummy {};
class NexradL2Message32: public NexradL2MessageDummy {};

#endif