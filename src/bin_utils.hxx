
#ifndef __METRSERVER_BINUTILS__
#define __METRSERVER_BINUTILS__

#include <iostream>

template <class T>
T READ(std::istream&);
std::string READ(std::istream&, int);

#endif