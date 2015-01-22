#ifndef PERIODLIB_ETC_H_
#define PERIODLIB_ETC_H_

#include <array>

int highest_bit(int n);

template<typename T, size_t N>
struct hash<std::array<T, N> >;

#endif  // PERIODLIB_ETC_H_
