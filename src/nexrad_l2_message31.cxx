
#include <nexrad_l2_message31.h>
#include <utils.h>
#include <nexrad_internal.h>

std::vector<std::string> NexradL2Message31::get_moments() const {
    std::vector<std::string> moments;

    for (auto it = this->moment_radials.begin(); it != this->moment_radials.end(); ++it) {
        moments.push_back(it->first);
    }

    return moments;
}

std::vector<float> NexradL2Message31::get_moment_data(const std::string& moment) const { 
    auto search = this->moment_radials.find(moment);

    if (search == this->moment_radials.end()) {
        throw std::string("No moment '") + moment + "' found.";
    }

    return search->second->get_moment_data();
}

std::string NexradL2Message31::dump() {
    std::string objstr = "NexradL2Message31 {\n";

    objstr += "    header = " + this->header->dump() + ",\n";
    objstr += "    radar_id = " + this->radar_id + ",\n";

    OUT_FIELD(radial_ms_since_midnight);
    OUT_FIELD(radial_julian_day);
    OUT_FIELD(radial_number);
    OUT_FIELD(radial_angle);

    OUT_FIELD(compression_type);

    OUT_FIELD(radial_length);
    OUT_FIELD(radial_spacing_code);
    OUT_FIELD(radial_status);
    OUT_FIELD(elevation_number);
    OUT_FIELD(cut_sector_number);
    OUT_FIELD(elevation_angle);
    OUT_FIELD(radial_blanking_status);
    OUT_FIELD(azimuth_indexing_mode);

    OUT_FIELD(number_of_blocks);
    OUT_FIELD(volume_meta_block_ptr);
    OUT_FIELD(elevation_meta_block_ptr);
    OUT_FIELD(radial_meta_block_ptr);
    OUT_FIELD(moment_block_ptrs[0]);
    OUT_FIELD(moment_block_ptrs[1]);
    OUT_FIELD(moment_block_ptrs[2]);
    OUT_FIELD(moment_block_ptrs[3]);
    OUT_FIELD(moment_block_ptrs[4]);
    OUT_FIELD(moment_block_ptrs[5]);

    OUT_FIELD(major_version);
    OUT_FIELD(minor_version);
    OUT_FIELD(latitude);
    OUT_FIELD(longtiude);
    OUT_FIELD(elevation);
    OUT_FIELD(feedhorn_height);
    OUT_FIELD(calibration_const);
    OUT_FIELD(horiz_tx_power);
    OUT_FIELD(vertical_tx_power);
    OUT_FIELD(zdr_calibration_const);
    OUT_FIELD(phidp_init);
    OUT_FIELD(vcp);
    OUT_FIELD(processing_flags);

    OUT_FIELD(atmos_atten_fac);
    OUT_FIELD(elevation_calibration_const);

    OUT_FIELD(unambiguous_range);
    OUT_FIELD(noise_horizontal);
    OUT_FIELD(noise_vertical);
    OUT_FIELD(nyquist_velocity);
    OUT_FIELD(radial_horiz_calibration_const);
    OUT_FIELD(radial_vert_calibration_const);

    for (auto const& item : this->moment_radials) {
        objstr += "    " + item.first + ": " + item.second->dump() + ",\n";
    }

    objstr.pop_back();
    objstr.pop_back();
    objstr += "\n}";

    return objstr;
}

std::istream& operator>>(std::istream& istream, NexradL2Message31& msg) {
    int start_of_msg_ptr = istream.tellg();

    msg.radar_id = READ(istream, 4);

    msg.radial_ms_since_midnight = READ<uint32_t>(istream);
    msg.radial_julian_day = READ<uint16_t>(istream);
    msg.radial_number = READ<uint16_t>(istream);
    msg.radial_angle = READ<float>(istream);

    msg.compression_type = READ<uint8_t>(istream);
    READ<uint8_t>(istream);

    msg.radial_length = READ<uint16_t>(istream);
    msg.radial_spacing_code = READ<uint8_t>(istream);
    msg.radial_status = READ<uint8_t>(istream);
    msg.elevation_number = READ<uint8_t>(istream);
    msg.cut_sector_number = READ<uint8_t>(istream);
    msg.elevation_angle = READ<float>(istream);
    msg.radial_blanking_status = READ<uint8_t>(istream);
    msg.azimuth_indexing_mode = READ<uint8_t>(istream);

    msg.number_of_blocks = READ<uint16_t>(istream);
    msg.volume_meta_block_ptr = READ<uint32_t>(istream);
    msg.elevation_meta_block_ptr = READ<uint32_t>(istream);
    msg.radial_meta_block_ptr = READ<uint32_t>(istream);

    msg.moment_block_ptrs = std::vector<uint32_t>(msg.number_of_blocks - 3);
    std::fill(msg.moment_block_ptrs.begin(), msg.moment_block_ptrs.end(), 0);
    
    for (int idx = 0; idx < msg.moment_block_ptrs.size(); idx++) {
        msg.moment_block_ptrs[idx] = READ<uint32_t>(istream);
    }

    istream.seekg(start_of_msg_ptr + msg.volume_meta_block_ptr);

    uint16_t block_size;

    READ(istream, 4);
    block_size = READ<uint16_t>(istream);
    msg.major_version = READ<uint8_t>(istream);
    msg.minor_version = READ<uint8_t>(istream);
    msg.latitude = READ<float>(istream);
    msg.longtiude = READ<float>(istream);
    msg.elevation = READ<int16_t>(istream);
    msg.feedhorn_height = READ<uint16_t>(istream);
    msg.calibration_const = READ<float>(istream);
    msg.horiz_tx_power = READ<float>(istream);
    msg.vertical_tx_power = READ<float>(istream);
    msg.zdr_calibration_const = READ<float>(istream);
    msg.phidp_init = READ<float>(istream);
    msg.vcp = READ<uint16_t>(istream);
    msg.processing_flags = READ<uint16_t>(istream);

    istream.seekg(start_of_msg_ptr + msg.elevation_meta_block_ptr);

    READ(istream, 4);
    block_size = READ<uint16_t>(istream);
    msg.atmos_atten_fac = READ<int16_t>(istream);
    msg.elevation_calibration_const = READ<float>(istream);

    istream.seekg(start_of_msg_ptr + msg.radial_meta_block_ptr);

    READ(istream, 4);
    block_size = READ<uint16_t>(istream);
    msg.unambiguous_range = READ<uint16_t>(istream);
    msg.noise_horizontal = READ<float>(istream);
    msg.noise_vertical = READ<float>(istream);
    msg.nyquist_velocity = READ<uint16_t>(istream);
    READ<uint16_t>(istream);
    msg.radial_horiz_calibration_const = READ<float>(istream);
    msg.radial_vert_calibration_const = READ<float>(istream);

    for (uint32_t mom_ptr : msg.moment_block_ptrs) {
        if (mom_ptr == 0) continue;

        istream.seekg(start_of_msg_ptr + mom_ptr);
        std::unique_ptr<NexradL2Message31MomentRadial> moment_radial = NexradL2Message31MomentRadial::factory(istream);
        msg.moment_radials[moment_radial->get_moment_name()] = std::move(moment_radial);
    }

    return istream;
}