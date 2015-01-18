#include "period.h"

#include <string.h>

#include "dbf.h"


// check if x is element of arithmetic progression
bool is_element(int x, int first, int diff, int last) {
    if (x < first || x > last)
        return false;
    return x == first + ((x - first) / diff) * diff;
}

bool is_borderless(const char * text, DBF & dbf, int start, int end) {

    // This implementation uses LargePS queries only
    // and does not find actual period length
    int n = end - start;

    // check 1-length basic factors
    if (text[start] == text[end - 1]) {
        return false;
    }

    // check basic factors
    for (int k = 2; k < n; k <<= 1) {
        int half = k >> 1;
        int left_half_id = dbf.id(start, half);
        int right_half_id = dbf.id(end - half, half);

        // first occurrence of right half in left factor
        int first_in_left = dbf.succ(start, half, right_half_id);
        if (first_in_left < 0 || first_in_left > start + half)
            // no occurrences
            continue;

        // first occurrence of left half in right factor
        int first_in_right = dbf.succ(end - k, half, left_half_id);
        if (first_in_right < 0 || first_in_right > end - half)
            // no occurrences
            continue;

        // second occurrence of right half in left factor
        int second_in_left = dbf.succ(first_in_left + 1, half, right_half_id);

        // last occurrence of right half in left factor
        int last_in_left = -1;
        if (second_in_left < 0 || second_in_left > start + half) {
            second_in_left = -1;
        } else {
            last_in_left = dbf.pred(start + half, half, right_half_id);
        }

        // second occurrence of left half in right factor
        int second_in_right = dbf.succ(first_in_right + 1, half, left_half_id);

        // last occurrence of left half in right factor
        int last_in_right = -1;
        if (second_in_right < 0 || second_in_right > end - half) {
            second_in_right = -1;
        } else {
            last_in_right = dbf.pred(end - half, half, left_half_id);
        }

        int first1 = half + first_in_left - start;
        int diff1 = second_in_left - first_in_left;
        int last1 = half + last_in_left - start;

        int first2 = end - first_in_right;
        int diff2 = second_in_right - first_in_right;
        int last2 = end - last_in_right;

        // check different cases
        if (second_in_left == -1 && second_in_right == -1) {
            if (first1 == first2)
                return false;
        } else if (second_in_left == -1) {
            if (is_element(first1, first2, diff2, last2))
                return false;
        } else if (second_in_right == -1) {
            if (is_element(first2, first1, diff1, last1))
                return false;
        } else {
            if (is_element(first2, first1, diff1, last1) || is_element(last2, first1, diff1, last1) ||
                    is_element(first1, first2, diff2, last2) || is_element(last1, first2, diff2, last2))
                return false;
        }
    }

    return true;
}

int max_borderless_length(const char * text) {
    DBF dbf(text);
    int n = (int) strlen(text);

    for (int i = n; i >= 2; --i) {
        for (int j = 0; j < n - i + 1; ++j) {
            if (is_borderless(text, dbf, j, j + i)) {
                return i;
            }
        }
    }

    return 1;
}
