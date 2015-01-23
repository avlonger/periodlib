#ifndef PERIOD_UTILITIES_H_
#define PERIOD_UTILITIES_H_

#include <string>

#include "dbf.h"

// Answers for given subword w[start, end - 1] of text
// with given DBF is it borderless or periodic
bool is_borderless(const char * text, DBF & dbf, int start, int end);

// Returns length of longest borderless subword of given word
// using Dictionary of Basic Factors data structure
int max_borderless_length_dbf(const char * text, int n = -1);

// Returns length of longest borderless subword of given word
// using Dictionary of Basic Factors + hash table data structure
int max_borderless_length_dbf_hashtable(const char * text, int n = -1);

// Returns longest borderless subword using border array based
// algorithm
int max_borderless_length_border(const char * text, int n = -1);

// Returns longest borderless subword using naive
// algorithm: test each subword of given word with stop
int max_borderless_length_naive(const char * text, int n = -1);

// Returns longest borderless subword using naive
// algorithm: test each subword of given word
int max_borderless_length_super_naive(const char * text, int n = -1);

// Returns longest borderless subword using border arrays
int max_borderless_length_border_fast(const char * text, int n = -1);

#endif  // PERIOD_UTILITES_H_
