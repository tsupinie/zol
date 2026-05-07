
#include <iostream>

#include <nexrad_l2_message31.h>
#include <nexrad_l2_message5.h>
#include <nexrad_l2_header.h>
#include <nexrad_internal.h>


NexradL2Message::NexradL2Message() {
}

NexradL2Message::~NexradL2Message() {
    delete this->header;
}

NexradL2Message* NexradL2Message::factory(std::istream& istream) {
    NexradL2MessageHeader* header = new NexradL2MessageHeader();
    istream >> *header;
    NexradL2Message* msg;

    if (header->get_message_type() == 0) {
        msg = new NexradL2Message0();
        istream >> *static_cast<NexradL2Message0*>(msg);
    }
    else if (header->get_message_type() == 2) {
        msg = new NexradL2Message2();
        istream >> *static_cast<NexradL2Message2*>(msg);
    }
    else if (header->get_message_type() == 3) {
        msg = new NexradL2Message3();
        istream >> *static_cast<NexradL2Message3*>(msg);
    }
    else if (header->get_message_type() == 5) {
        msg = new NexradL2Message5();
        istream >> *static_cast<NexradL2Message5*>(msg);
    }
    else if (header->get_message_type() == 8) {
        msg = new NexradL2Message8();
        istream >> *static_cast<NexradL2Message8*>(msg);
    }
    else if (header->get_message_type() == 13) {
        msg = new NexradL2Message13();
        istream >> *static_cast<NexradL2Message13*>(msg);
    }
    else if (header->get_message_type() == 15) {
        msg = new NexradL2Message15();
        istream >> *static_cast<NexradL2Message15*>(msg);
    }
    else if (header->get_message_type() == 18) {
        msg = new NexradL2Message18();
        istream >> *static_cast<NexradL2Message18*>(msg);
    }
    else if (header->get_message_type() == 31) {
        msg = new NexradL2Message31();
        istream >> *static_cast<NexradL2Message31*>(msg);
    }
    else if (header->get_message_type() == 32) {
        msg = new NexradL2Message32();
        istream >> *static_cast<NexradL2Message32*>(msg);
    }
    else {
        throw "Unknown message_type " + std::to_string(header->get_message_type());
    }

    msg->header = header;

    return msg;
}

std::string NexradL2MessageDummy::dump() {
    return "NexradL2Message" + std::to_string(this->header->get_message_type()) 
           + " {\n    header = " + this->header->dump() + "\n}";
}

std::istream& operator>>(std::istream& istream, NexradL2MessageDummy& msg) {
    istream.seekg(MSG_SIZE - NETWORK_HEADER_SIZE - MESG_HEADER_SIZE, std::ios_base::cur);

    return istream;
}