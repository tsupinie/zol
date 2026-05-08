
#include <nexrad_l2_header.h>
#include <utils.h>

#define OUT_FIELD_INLINE(x) objstr += #x " = " + std::to_string(this->x) + ", "

std::istream& operator>>(std::istream& istream, NexradL2MessageHeader& msg) {
    msg.msg_size = READ<uint16_t>(istream);
    msg.rda_channel = READ<uint8_t>(istream);
    msg.msg_type = READ<uint8_t>(istream);

    msg.msg_sequence_num = READ<uint16_t>(istream);
    msg.julian_day = READ<uint16_t>(istream);
    msg.ms_since_midnight = READ<uint32_t>(istream);
    msg.number_of_segs = READ<uint16_t>(istream);
    msg.seg_num = READ<uint16_t>(istream);

    return istream;
}

std::string NexradL2MessageHeader::dump() {
    std::string objstr = "NexradL2MessageHeader {";

    OUT_FIELD_INLINE(msg_size);
    OUT_FIELD_INLINE(rda_channel);
    OUT_FIELD_INLINE(msg_type);
    OUT_FIELD_INLINE(msg_sequence_num);
    OUT_FIELD_INLINE(julian_day);
    OUT_FIELD_INLINE(ms_since_midnight);
    OUT_FIELD_INLINE(number_of_segs);
    OUT_FIELD_INLINE(seg_num);

    objstr.pop_back();
    objstr.pop_back();
    objstr += "}";

    return objstr;
}