
#include "nexrad_l2_message5.hxx"
#include "bin_utils.hxx"
#include "nexrad_internal.hxx"

NexradL2Message5::~NexradL2Message5() {
    for (NexradL2Message5ElevationMeta* elev : this->elevations) {
        delete elev;
    }
    this->elevations.clear();
}

std::string NexradL2Message5::dump() {
    std::string objstr = "NexradL2Message5 {\n";

    objstr += "    header = " + this->header->dump() + ",\n";
    
    OUT_FIELD(pattern_type);
    OUT_FIELD(pattern_number);
    OUT_FIELD(n_elevations);
    OUT_FIELD(vcp_version);
    OUT_FIELD(clutter_map_group);
    OUT_FIELD(doppler_vel_res);
    OUT_FIELD(pulse_width);
    OUT_FIELD(vcp_sequence_flags);
    OUT_FIELD(vcp_supplemental_flags);

    for (NexradL2Message5ElevationMeta* elev : this->elevations) {
        objstr += "    " + elev->dump() + ",\n";
    }

    objstr.pop_back();
    objstr.pop_back();
    objstr += "\n}";

    return objstr;
}

std::istream& operator>>(std::istream& istream, NexradL2Message5& msg) {
    int stream_pos = istream.tellg();

    READ<uint16_t>(istream);
    msg.pattern_type = READ<uint16_t>(istream);
    msg.pattern_number = READ<uint16_t>(istream);
    msg.n_elevations = READ<uint16_t>(istream);
    msg.vcp_version = READ<uint8_t>(istream);
    msg.clutter_map_group = READ<uint8_t>(istream);
    msg.doppler_vel_res = READ<uint8_t>(istream);
    msg.pulse_width = READ<uint8_t>(istream);
    READ<uint16_t>(istream);

    msg.vcp_sequence_flags = READ<uint16_t>(istream);
    msg.vcp_supplemental_flags = READ<uint16_t>(istream);
    READ<uint32_t>(istream);

    msg.elevations = std::vector<NexradL2Message5ElevationMeta*>(msg.n_elevations);
    for (int ielev = 0; ielev < msg.elevations.size(); ielev++) {
        NexradL2Message5ElevationMeta* elev = new NexradL2Message5ElevationMeta();
        istream >> *elev;
        msg.elevations[ielev] = elev;
    }

    istream.seekg(stream_pos + MSG_SIZE - NETWORK_HEADER_SIZE);
    return istream;
}

std::vector<float> NexradL2Message5::get_elevation_angles() const {
    std::vector<float> retval;

    for (NexradL2Message5ElevationMeta* elv : this->elevations) {
        retval.push_back(elv->get_elevation_angle());
    }

    retval.shrink_to_fit();
    return retval;
}

std::string NexradL2Message5ElevationMeta::dump() {
    std::string objstr = "NexradL2Messsage5ElevationMeta {";

    OUT_FIELD_INLINE(elevation_angle);
    OUT_FIELD_INLINE(channel_config);
    OUT_FIELD_INLINE(waveform_type);
    OUT_FIELD_INLINE(super_res_flags);
    OUT_FIELD_INLINE(surveillance_prf);
    OUT_FIELD_INLINE(surveillance_pulse_count);
    OUT_FIELD_INLINE(azimuth_rate);

    OUT_FIELD_INLINE(snr_ref_threshold);
    OUT_FIELD_INLINE(snr_vel_threshold);
    OUT_FIELD_INLINE(snr_sw_threshold);
    OUT_FIELD_INLINE(snr_zdr_threshold);
    OUT_FIELD_INLINE(snr_phi_threshold);
    OUT_FIELD_INLINE(snr_rhv_threshold);

    OUT_FIELD_INLINE(sector1_edge_angle);
    OUT_FIELD_INLINE(sector1_prf);
    OUT_FIELD_INLINE(sector1_pulse_count);
    OUT_FIELD_INLINE(sector2_edge_angle);
    OUT_FIELD_INLINE(sector2_prf);
    OUT_FIELD_INLINE(sector2_pulse_count);
    OUT_FIELD_INLINE(sector3_edge_angle);
    OUT_FIELD_INLINE(sector3_prf);
    OUT_FIELD_INLINE(sector3_pulse_count);

    OUT_FIELD_INLINE(supplemental);
    OUT_FIELD_INLINE(ebc_angle);

    objstr.pop_back();
    objstr.pop_back();
    objstr += "}";

    return objstr;
}

std::istream& operator>>(std::istream& istream, NexradL2Message5ElevationMeta& msg) {
    msg.elevation_angle = READ<uint16_t>(istream);

    msg.channel_config = READ<uint8_t>(istream);
    msg.waveform_type = READ<uint8_t>(istream);
    msg.super_res_flags = READ<uint8_t>(istream);
    msg.surveillance_prf = READ<uint8_t>(istream);
    msg.surveillance_pulse_count = READ<uint16_t>(istream);
    msg.azimuth_rate = READ<uint16_t>(istream);

    msg.snr_ref_threshold = READ<int16_t>(istream);
    msg.snr_vel_threshold = READ<int16_t>(istream);
    msg.snr_sw_threshold = READ<int16_t>(istream);
    msg.snr_zdr_threshold = READ<int16_t>(istream);
    msg.snr_phi_threshold = READ<int16_t>(istream);
    msg.snr_rhv_threshold = READ<int16_t>(istream);

    msg.sector1_edge_angle = READ<uint16_t>(istream);
    msg.sector1_prf = READ<uint16_t>(istream);
    msg.sector1_pulse_count = READ<uint16_t>(istream);

    msg.supplemental = READ<uint16_t>(istream);

    msg.sector2_edge_angle = READ<uint16_t>(istream);
    msg.sector2_prf = READ<uint16_t>(istream);
    msg.sector2_pulse_count = READ<uint16_t>(istream);

    msg.ebc_angle = READ<uint16_t>(istream);

    msg.sector3_edge_angle = READ<uint16_t>(istream);
    msg.sector3_prf = READ<uint16_t>(istream);
    msg.sector3_pulse_count = READ<uint16_t>(istream);

    READ<uint16_t>(istream);

    return istream;
}