#include "dbf.h"

#include <set>
#include <map>
#include <string>

#include <iostream>

#include "etc.h"


using namespace std;


DBF::DBF(const string & text) {

    int n = (int)text.length();

    // find all distinct letters
    set<char> chars(text.begin(), text.end());

    // build translation dictionary: letter => 1-length factor id
    map<char, int> dict;
    int char_count = 0;
    for (set<char>::iterator it = chars.begin(); it != chars.end(); ++it, ++char_count) {
        dict[*it] = char_count;
    }

    // fill ids for factors of length 1
    ids.push_back(vector<int>(n));
    for (int i = 0; i < n; ++i) {
        ids[0][i] = dict[text[i]];
    }

    // sort-rename log n times
    int k, power;
    for (k = 0, power = 1; power << 1 < n; k += 1, power <<= 1) {

        // fill positions for factors of length i TODO make function for that
    }

}


DBF::~DBF() {
}


int DBF::succ(int i, int k, int id){
    return 0;
}


int DBF::pred(int i, int k, int id) {
    return 0;
}

int DBF::id(int i, int k) {
    return ids[highest_bit(k)][i];
}
