
#include <nexrad_l2_message31_momrad.h>
#include <utils.h>
#include <nexrad_internal.h>

std::unique_ptr<NexradL2Message31MomentRadial> NexradL2Message31MomentRadial::factory(std::istream& istream) {
    int start_of_msg_ptr = istream.tellg();
    uint8_t word_size;

    istream.seekg(19, std::ios_base::cur);
    word_size = READ<uint8_t>(istream);
    istream.seekg(start_of_msg_ptr);

    std::unique_ptr<NexradL2Message31MomentRadial> mom_rad;

    if (word_size == 8) {
        mom_rad = make_unique_ptr_from_stream<NexradL2Message31MomentRadial_<uint8_t>>(istream);
    }
    else if (word_size == 16) {
        mom_rad = make_unique_ptr_from_stream<NexradL2Message31MomentRadial_<uint16_t>>(istream);
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

template <class T>
std::vector<float> NexradL2Message31MomentRadial_<T>::get_moment_data() const {
    std::vector<float> data(this->moment_data.size());

    for (size_t i = 0 ; i < data.size() ; i++) {
        data[i] = (this->moment_data[i] - this->offset) / this->scale;
    }

    return data;
}

template class NexradL2Message31MomentRadial_<uint8_t>;
template class NexradL2Message31MomentRadial_<uint16_t>;