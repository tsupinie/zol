
#include <string>
#include <cstdint>
#include <vector>

#ifndef __NEXRAD_L2_MESSAGE31_MOMRAD__
#define __NEXRAD_L2_MESSAGE31_MOMRAD__

class NexradL2Message31MomentRadial {
    public:
        virtual ~NexradL2Message31MomentRadial() {}
        static NexradL2Message31MomentRadial* factory(std::istream&);
        virtual std::string dump() = 0;

        std::string get_moment_name() const { return this->moment_name; }
        std::vector<float> get_moment_data() const { return this->get_scaled_moment_data(this->offset, this->scale); };

    protected:
        virtual std::vector<float> get_scaled_moment_data(const float, const float) const = 0;

        std::string moment_name;
        uint16_t num_gates;
        uint16_t range_to_first_gate;
        uint16_t gate_spacing;
        uint16_t overlay_threshold;
        int16_t snr_threshold;
        uint8_t control_flags;
        uint8_t word_size;
        float scale;
        float offset;
};

template <class T>
class NexradL2Message31MomentRadial_ : public NexradL2Message31MomentRadial {
    public:        
        template <class U> 
        friend std::istream& operator>>(std::istream&, NexradL2Message31MomentRadial_<U>&);

        std::string dump();

    protected:
        std::vector<float> get_scaled_moment_data(const float, const float) const;

        std::vector<T> moment_data;
};

#endif