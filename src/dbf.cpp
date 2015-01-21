#include "dbf.h"

#include <string.h>

#include <set>
#include <map>
#include <numeric>
#include <string>
#include <algorithm>

#include "etc.h"


using namespace std;


DBF::DBF(const char * text, int n) {

    if (n == -1) {
        n = (int) strlen(text);
    }

    // find all distinct letters
    set<char> chars(text, text + n);

    // build translation dictionary: letter => 1-length factor id
    map<char, int> dict;
    int char_count = 0;
    for (set<char>::iterator it = chars.begin(); it != chars.end(); ++it, ++char_count) {
        dict[*it] = char_count;
    }

    // fill ids and positions for factors of length 1
    ids.push_back(vector<int>(n));
    pos.push_back(vector< vector<int> >(chars.size()));
    for (int i = 0; i < n; ++i) {
        ids[0][i] = dict[text[i]];
        pos[0][ids[0][i]].push_back(i);
    }

    // sort-rename log n times
    int k, power, ids_count = (int)dict.size(), id, id2;
    vector<int> buffer(n + 1), buffer2(n + 1), buckets(n + 1), used(n + 1);

    for (k = 0, power = 1; power << 1 < n; k += 1, power <<= 1) {

        // sort by the second digit
        std::fill(buckets.begin(), buckets.end(), 0);
        std::fill(used.begin(), used.end(), 0);

        // at first find bucket sizes
        for (int i = power; i < n; ++i)
            buckets[ids[k][i] + 1]++;

        // then find buckets starting positions
        std::partial_sum(buckets.begin(), buckets.begin() + ids_count + 1, buckets.begin());

        // actually sort by second digit
        for (int i = 0; i + power < n; ++i) {
            id = ids[k][i + power];
            buffer[buckets[id] + used[id]++] = i;
        }
        for (int i = (int) n - power; i < n; ++i) {
            buffer[buckets[ids_count] + used[ids_count]++] = i;
        }

        // sort by the first digit
        ids.push_back(vector<int>(n));
        std::fill(buckets.begin(), buckets.end(), 0);
        std::fill(used.begin(), used.end(), 0);

        // at first find bucket sizes
        for (int i = 0; i < n; ++i)
            buckets[ids[k][i] + 1]++;

        // then find buckets starting positions
        std::partial_sum(buckets.begin(), buckets.begin() + ids_count, buckets.begin());

        // sort
        for (int i = 0; i < n; ++i) {
            id = ids[k][buffer[i]];
            buffer2[buckets[id] + used[id]++] = buffer[i];
        }

        // create new ids
        id = ids[k][buffer2[0]];
        id2 = (buffer2[0] + power < n) ? ids[k][buffer2[0] + power] : ids_count;

        int current_id = 0;
        int next_id;
        ids[k + 1][buffer2[0]] = current_id;
        for (int i = 1; i < n; ++i) {
            next_id = (buffer2[i] + power < n) ? ids[k][buffer2[i] + power] : ids_count;
            if (ids[k][buffer2[i]] != id || next_id != id2) {
                id = ids[k][buffer2[i]];
                id2 = next_id;
                current_id++;
            }
            ids[k + 1][buffer2[i]] = current_id;
        }
        ids_count = current_id + 1;

        fill_positions(k + 1, ids_count);
    }
}

void DBF::fill_positions(int k, int ids_count) {
    if (pos.size() < k + 1) {
        pos.push_back(vector< vector<int> >((unsigned long)ids_count));
    } else {
        pos[k].clear();
    }
    int i = 0;
    for (vector<int>::iterator it = ids[k].begin(); it != ids[k].end(); ++it, ++i) {
        pos[k][*it].push_back(i);
    }
}


int DBF::succ(int i, int k, int id){
    int log2 = highest_bit(k);
    vector<int>::iterator result = std::lower_bound(
        pos[log2][id].begin(), pos[log2][id].end(), i
    );
    if (result == pos[log2][id].end())
        return -1;
    return *result;
}


int DBF::pred(int i, int k, int id) {
    int log2 = highest_bit(k);
    vector<int>::iterator result = std::upper_bound(
        pos[log2][id].begin(), pos[log2][id].end(), i
    );
    if (result == pos[log2][id].end())
        return -1;
    return *result;
}


int DBF::id(int i, int k) {
    return ids[highest_bit(k)][i];
}


DBFHashTable::DBFHashTable(const char * text, int n) : DBF(text, n) {}


void DBFHashTable::fill_positions(int k, int ids_count) {
    if (pos.size() < k + 1) {
        pos.push_back(vector< unordered_map<int, triplet> >((unsigned long)ids_count));
    } else {
        pos[k].clear();
    }

    int n = (int) ids[k].size();

    // factor size
    int size = 1 << k;

    int slice_pos = 0;
    for (int slice = 0; slice < n; slice += size, ++slice_pos) {
        for (int i = slice; i < slice + size && i < n; ++i) {
            unordered_map<int, triplet> & occ_map = pos[k][ids[k][i]];
            unordered_map<int, triplet>::iterator it = occ_map.find(slice_pos);
            if (it == occ_map.end()) {
                occ_map[slice_pos] = triplet{i, -1, -1};
            } else {
                // save difference of arithmetic progression value
                if (it->second[2] == -1) {
                    it->second[2] = i - it->second[0];
                }
                it->second[1] = i;
            }
        }
    }
}


int DBFHashTable::succ_short(int i, int k, int id) {

}

int DBFHashTable::pred_short(int i, int k, int id) {

}
