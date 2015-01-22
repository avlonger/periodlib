#include "etc.h"

#include <array>

int highest_bit(int n) {
    int r = 0;
    while (n >>= 1) {
        ++r;
    }
    return r;
}

namespace std
{
    template<typename T, size_t N>
    struct hash<array<T, N> >
{
    typedef array<T, N> argument_type;
    typedef size_t result_type;

    result_type operator()(const argument_type& a) const
    {
        hash<T> hasher;
        result_type h = 0;
        for (result_type i = 0; i < N; ++i)
        {
            h = h * 4093 + hasher(a[i]);
        }
        return h;
    }
};
}
