
#include <nexrad_l2_message31_momrad.h>
#include <bin_utils.h>
#include <nexrad_internal.h>

NexradL2Message31MomentRadial* NexradL2Message31MomentRadial::factory(std::istream& istream) {
    int start_of_msg_ptr = istream.tellg();
    uint8_t word_size;

    istream.seekg(19, std::ios_base::cur);
    word_size = READ<uint8_t>(istream);
    istream.seekg(start_of_msg_ptr);

    NexradL2Message31MomentRadial* mom_rad;

    if (word_size == 8) {
        mom_rad = new NexradL2Message31MomentRadial_<uint8_t>();
        istream >> *static_cast<NexradL2Message31MomentRadial_<uint8_t>*>(mom_rad);
    }
    else if (word_size == 16) {
        mom_rad = new NexradL2Message31MomentRadial_<uint16_t>();
        istream >> *static_cast<NexradL2Message31MomentRadial_<uint16_t>*>(mom_rad);
    }
    else {
        throw "Unknown moment radial word size: " + std::to_string(word_size);
    }
    return mom_rad;
}

template <class T>
std::string NexradL2Message31MomentRadial_<T>::dump() {
    std::string objstr = "NexradL2Message31MomentRadial {";

    objstr += "moment_name = " + this->moment_name + ", ";
    OUT_FIELD_INLINE(num_gates);
    OUT_FIELD_INLINE(range_to_first_gate);
    OUT_FIELD_INLINE(gate_spacing);
    OUT_FIELD_INLINE(overlay_threshold);
    OUT_FIELD_INLINE(snr_threshold);
    OUT_FIELD_INLINE(control_flags);
    OUT_FIELD_INLINE(word_size);
    OUT_FIELD_INLINE(scale);
    OUT_FIELD_INLINE(offset);

    objstr.pop_back();
    objstr.pop_back();
    objstr += "}";

    return objstr;
}

template <class T>
std::istream& operator>>(std::istream& istream, NexradL2Message31MomentRadial_<T>& msg) {
    uint32_t dummy32;

    std::string marker = READ(istream, 4);
    msg.moment_name = marker.substr(1);
    READ<uint32_t>(istream);

    msg.num_gates = READ<uint16_t>(istream);
    msg.range_to_first_gate = READ<uint16_t>(istream);
    msg.gate_spacing = READ<uint16_t>(istream);
    msg.overlay_threshold = READ<uint16_t>(istream);
    msg.snr_threshold = READ<uint16_t>(istream);
    msg.control_flags = READ<uint8_t>(istream);
    msg.word_size = READ<uint8_t>(istream);
    msg.scale = READ<float>(istream);
    msg.offset = READ<float>(istream);

    msg.moment_data = std::vector<T>(msg.num_gates);
    for (int igate = 0; igate < msg.moment_data.size(); igate++) {
        msg.moment_data[igate] = READ<T>(istream);
    }

    return istream;
}

template class NexradL2Message31MomentRadial_<uint8_t>;
template class NexradL2Message31MomentRadial_<uint16_t>;