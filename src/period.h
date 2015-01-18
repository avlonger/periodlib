#ifndef PERIOD_UTILITIES_H_
#define PERIOD_UTILITIES_H_

#include <string>

#include "dbf.h"

// Answers for given subword w[start, end - 1] of text
// with given DBF is it borderless or periodic
bool is_borderless(const std::string & text, DBF & dbf, int start, int end);

#endif  // PERIOD_UTILITES_H_
