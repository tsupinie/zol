
#include <string>
#include <cstdint>
#include <memory>

#include "nexrad_l2_message.h"
#include "nexrad_l2_message31_momrad.h"

#ifndef __NEXRAD_L2_MESSAGE31__
#define __NEXRAD_L2_MESSAGE31__

class NexradL2Message31 : public NexradL2Message {
    public:
        friend std::istream& operator>>(std::istream&, NexradL2Message31&);

        std::vector<float> get_moment_data(const std::string&) const;
        size_t get_n_gates() const { return this->radial_length; }

        std::string dump();

    private:
        std::string radar_id;

        uint32_t radial_ms_since_midnight;
        uint16_t radial_julian_day;
        uint16_t radial_number;
        float radial_angle;

        uint8_t compression_type;

        uint16_t radial_length;
        uint8_t radial_spacing_code;
        uint8_t radial_status;
        uint8_t elevation_number;
        uint8_t cut_sector_number;
        float elevation_angle;
        uint8_t radial_blanking_status;
        uint8_t azimuth_indexing_mode;

        uint16_t number_of_blocks;
        uint32_t volume_meta_block_ptr;
        uint32_t elevation_meta_block_ptr;
        uint32_t radial_meta_block_ptr;
        std::vector<uint32_t> moment_block_ptrs;

        uint8_t major_version;
        uint8_t minor_version;
        float latitude;
        float longtiude;
        int16_t elevation;
        uint16_t feedhorn_height;
        float calibration_const;
        float horiz_tx_power;
        float vertical_tx_power;
        float zdr_calibration_const;
        float phidp_init;
        uint16_t vcp;
        uint16_t processing_flags;

        int16_t atmos_atten_fac;
        float elevation_calibration_const;

        uint16_t unambiguous_range;
        float noise_horizontal;
        float noise_vertical;
        uint16_t nyquist_velocity;
        float radial_horiz_calibration_const;
        float radial_vert_calibration_const;

        std::map<std::string, std::unique_ptr<NexradL2Message31MomentRadial>> moment_radials;
};

#endif