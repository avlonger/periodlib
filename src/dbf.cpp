#include "dbf.h"

#include <set>
#include <map>
#include <numeric>
#include <string>

#include "etc.h"


using namespace std;


DBF::DBF(const string & text) {

    unsigned long n = text.length();

    // find all distinct letters
    set<char> chars(text.begin(), text.end());

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
        pos.push_back(vector< vector<int> >((unsigned long)ids_count));
        for (int i = 0; i < n; ++i) {
            pos[k + 1][ids[k + 1][i]].push_back(i);
        }
    }
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
