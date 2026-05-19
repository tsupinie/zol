
#include <iostream>

#include <zol/nexrad_l2_message31.h>
#include <zol/nexrad_l2_message5.h>
#include <zol/nexrad_l2_header.h>
#include <zol/nexrad_internal.h>
#include <zol/utils.h>


std::unique_ptr<NexradL2Message> NexradL2Message::factory(std::istream& istream) {
    std::unique_ptr<NexradL2Message> msg;
    std::unique_ptr<NexradL2MessageHeader> header = make_unique_ptr_from_stream<NexradL2MessageHeader>(istream);

    if (header->get_message_type() == 0) {
        msg = make_unique_ptr_from_stream<NexradL2Message0>(istream);
    }
    else if (header->get_message_type() == 2) {
        msg = make_unique_ptr_from_stream<NexradL2Message2>(istream);
    }
    else if (header->get_message_type() == 3) {
        msg = make_unique_ptr_from_stream<NexradL2Message3>(istream);
    }
    else if (header->get_message_type() == 5) {
        msg = make_unique_ptr_from_stream<NexradL2Message5>(istream);
    }
    else if (header->get_message_type() == 8) {
        msg = make_unique_ptr_from_stream<NexradL2Message8>(istream);
    }
    else if (header->get_message_type() == 13) {
        msg = make_unique_ptr_from_stream<NexradL2Message13>(istream);
    }
    else if (header->get_message_type() == 15) {
        msg = make_unique_ptr_from_stream<NexradL2Message15>(istream);
    }
    else if (header->get_message_type() == 18) {
        msg = make_unique_ptr_from_stream<NexradL2Message18>(istream);
    }
    else if (header->get_message_type() == 31) {
        msg = make_unique_ptr_from_stream<NexradL2Message31>(istream);
    }
    else if (header->get_message_type() == 32) {
        msg = make_unique_ptr_from_stream<NexradL2Message32>(istream);
    }
    else {
        throw "Unknown message_type " + std::to_string(header->get_message_type());
    }

    msg->header = std::move(header);

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