
#ifndef __NEXRAD_INTERNAL__
#define __NEXRAD_INTERNAL__

#define MSG_SIZE 2432
#define NETWORK_HEADER_SIZE 12
#define MESG_HEADER_SIZE 16

#define OUT_FIELD(x) objstr += "    " #x " = " + std::to_string(this->x) + ",\n"
#define OUT_FIELD_INLINE(x) objstr += #x " = " + std::to_string(this->x) + ", "

#endif