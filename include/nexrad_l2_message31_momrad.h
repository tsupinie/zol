
#include <string>
#include <cstdint>
#include <vector>
#include <memory>

#ifndef __NEXRAD_L2_MESSAGE31_MOMRAD__
#define __NEXRAD_L2_MESSAGE31_MOMRAD__

class NexradL2Message31MomentRadial {
    public:
        static std::unique_ptr<NexradL2Message31MomentRadial> factory(std::istream&);
        virtual std::string dump() = 0;
        virtual std::vector<float> get_moment_data() const = 0;
        virtual std::string get_moment_name() const = 0;
};

template <class T>
class NexradL2Message31MomentRadial_ : public NexradL2Message31MomentRadial {
    public:        
        template <class U> 
        friend std::istream& operator>>(std::istream&, NexradL2Message31MomentRadial_<U>&);

        std::string get_moment_name() const { return this->moment_name; }
        std::vector<float> get_moment_data() const;

        std::string dump();

    private:
        std::string moment_name {""};
        uint16_t num_gates {0};
        uint16_t range_to_first_gate {0};
        uint16_t gate_spacing {0};
        uint16_t overlay_threshold {0};
        int16_t snr_threshold {0};
        uint8_t control_flags {0};
        uint8_t word_size {0};
        float scale {0};
        float offset {0};

        std::vector<T> moment_data {};
};

#endif