#ifndef DICTIONARY_OF_BASIC_FACTORS_H_
#define DICTIONARY_OF_BASIC_FACTORS_H_

#include <vector>
#include <string>

// Dictionary of Basic Factors
// M.Crochemore "Jewels of Stringology", p.85
class DBF {
public:
    // Creates a new DBF data structure for given text
    // using Karp-Miller-Rosenberg algorithm
    // it requires O(n log n) time and space
    DBF(const char * text, int n = -1);

    // Returns the first occurrence of
    // basic factor having given id and length k
    // that is not less than i
    int succ(int i, int k, int id);

    // Returns the last occurrence of
    // basic factor having given id and length k
    // that is not greater than i
    int pred(int i, int k, int id);

    // Returns id of basic factor
    // starting in position i
    // having length k
    int id(int i, int k);

private:
    // ids of basic factors
    std::vector< std::vector<int> > ids;

    // positions of basic factors
    std::vector< std::vector< std::vector<int> > > pos;

};

#endif  // DICTIONARY_OF_BASIC_FACTORS_H_