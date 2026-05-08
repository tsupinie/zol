
#ifndef __ZOL_UTILS__
#define __ZOL_UTILS__

#include <iostream>
#include <memory>

template <class T>
T READ(std::istream&);
std::string READ(std::istream&, int);

template <class T>
std::unique_ptr<T> make_unique_ptr_from_stream(std::istream& istream);

#endif