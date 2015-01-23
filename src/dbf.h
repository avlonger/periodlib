#ifndef DICTIONARY_OF_BASIC_FACTORS_H_
#define DICTIONARY_OF_BASIC_FACTORS_H_

#include <vector>
#include <string>
#include <array>
#include <map>
#include <unordered_map>

// Dictionary of Basic Factors
// M.Crochemore "Jewels of Stringology", p.85
class DBF {
public:
    // default constructor
    DBF();

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

    // Returns the last occurrence of
    // basic factor v having given id from range
    // [i - |v|, i]
    virtual int pred_short(int i, int k, int id);

    // Returns the last occurrence of
    // basic factor v having given id from range
    // [i, i + |v|]
    virtual int succ_short(int i, int k, int id);

protected:

    virtual void __build_dbf(const char * text, int n);

    // fill positions data structure with
    // freshly-generated basic factor ids
    virtual void fill_positions(int k, int ids_count);

    // ids of basic factors
    std::vector< std::vector<int> > ids;

    // positions of basic factors
    std::vector< std::vector< std::vector<int> > > pos;

};

// Dictionary of Basic Factors
// and basic factor occurrences hash table
class DBFHashTable : public DBF{
public:

    // type for arithmetic progression:
    // (first, last, diff)
    typedef std::array<int, 3> triplet;

    // default constructor
    DBFHashTable();

    // Creates a new DBF data structure and hash table
    // for constant-time succ and pred queries
    DBFHashTable(const char * text, int n = -1);

    // Returns the first occurrence of
    // basic factor v having given id from range
    // [i - |v|, i]
    virtual int pred_short(int i, int k, int id);

    // Returns the first occurrence of
    // basic factor v having given id from range
    // [i, i + |v|]
    virtual int succ_short(int i, int k, int id);

private:

    typedef std::map<int, triplet> table_type;

    // fill positions data structure with
    // freshly-generated basic factor ids
    virtual void fill_positions(int k, int ids_count);

    // positions of basic factors
    std::vector< std::vector< table_type > > pos;

    // remove long SUCC and PREV queries from derived class
    using DBF::succ;
    using DBF::pred;

};

#endif  // DICTIONARY_OF_BASIC_FACTORS_H_