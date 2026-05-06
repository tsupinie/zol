
#include "bin_utils.hxx"

template <class T>
T READ(std::istream& istream) {
    char buf[sizeof(T)];
    istream.read(buf, sizeof(T));

    if (sizeof(T) > 1) {
        for (int i = 0; i < sizeof(T) / 2; i++) {
            std::swap(buf[i], buf[sizeof(T) - i - 1]);
        }
    }

    return *reinterpret_cast<T*>(buf);
}

std::string READ(std::istream& istream, int n_bytes) {
    std::string retval = std::string(n_bytes, '\0');
    istream.read(&retval[0], n_bytes);
    return retval;
}

template uint32_t READ<>(std::istream&);
template int32_t READ<>(std::istream&);
template uint16_t READ<>(std::istream&);
template int16_t READ<>(std::istream&);
template uint8_t READ<>(std::istream&);
template int8_t READ<>(std::istream&);
template float READ<>(std::istream&);