
#include <string>
#include <cstdint>

#include "nexrad_l2_message.h"

#ifndef __NEXRAD_L2_MESSAGE5__
#define __NEXRAD_L2_MESSAGE5__

class NexradL2Message5ElevationMeta {
    public:
        friend std::istream& operator>>(std::istream&, NexradL2Message5ElevationMeta&);
        std::string dump();

        float get_elevation_angle() const { return (this->elevation_angle >> 3) * 0.043945; };

    private:
        uint16_t elevation_angle;
        uint8_t channel_config;
        uint8_t waveform_type;
        uint8_t super_res_flags;
        uint8_t surveillance_prf;
        uint16_t surveillance_pulse_count;
        uint16_t azimuth_rate;

        int16_t snr_ref_threshold;
        int16_t snr_vel_threshold;
        int16_t snr_sw_threshold;
        int16_t snr_zdr_threshold;
        int16_t snr_phi_threshold;
        int16_t snr_rhv_threshold;

        uint16_t sector1_edge_angle;
        uint16_t sector1_prf;
        uint16_t sector1_pulse_count;
        uint16_t sector2_edge_angle;
        uint16_t sector2_prf;
        uint16_t sector2_pulse_count;
        uint16_t sector3_edge_angle;
        uint16_t sector3_prf;
        uint16_t sector3_pulse_count;

        uint16_t supplemental;
        uint16_t ebc_angle;
};

class NexradL2Message5 : public NexradL2MessageType<5> {
    public:
        ~NexradL2Message5();

        friend std::istream& operator>>(std::istream&, NexradL2Message5&);
        std::string dump();

        std::vector<float> get_elevation_angles() const;

    private:
        uint16_t pattern_type;
        uint16_t pattern_number;
        uint16_t n_elevations;
        uint8_t vcp_version;
        uint8_t clutter_map_group;
        uint8_t doppler_vel_res;
        uint8_t pulse_width;
        uint16_t vcp_sequence_flags;
        uint16_t vcp_supplemental_flags;

        std::vector<NexradL2Message5ElevationMeta*> elevations;
};

#endif