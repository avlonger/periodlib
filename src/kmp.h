#ifndef KNUTH_MORRIS_PRATT_H_
#define KNUTH_MORRIS_PRATT_H_

// Knuth-Moriss-Pratt algorithm
int kmp(const char * pattern, const char * text, int * output);

// Build border array for given text
void border(const char * text, int * border, int n = -1);

#endif  // KNUTH_MORRIS_PRATT_H_