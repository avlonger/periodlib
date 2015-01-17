#include "etc.h"

int highest_bit(int n) {
    int r = 0;
    while (n >>= 1) {
        ++r;
    }
    return r;
}
