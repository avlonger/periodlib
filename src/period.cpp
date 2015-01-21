#include "period.h"

#include <stdlib.h>
#include <string.h>

#include "kmp.h"
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
        int first_in_left = dbf.succ_short(start, half, right_half_id);
        if (first_in_left < 0)
            // no occurrences
            continue;

        // first occurrence of left half in right factor
        int first_in_right = dbf.succ_short(end - k, half, left_half_id);
        if (first_in_right < 0)
            // no occurrences
            continue;

        // second occurrence of right half in left factor
        int second_in_left = dbf.succ_short(first_in_left + 1, half, right_half_id);

        // last occurrence of right half in left factor
        int last_in_left = first_in_left;
        if (second_in_left < 0) {
            second_in_left = -1;
        } else {
            last_in_left = dbf.pred_short(start + half, half, right_half_id);
        }

        // second occurrence of left half in right factor
        int second_in_right = dbf.succ_short(first_in_right + 1, half, left_half_id);

        // last occurrence of left half in right factor
        int last_in_right = first_in_right;
        if (second_in_right < 0) {
            second_in_right = -1;
        } else {
            last_in_right = dbf.pred_short(end - k, half, left_half_id);
        }

        int first1 = half + first_in_left - start;
        int diff1 = second_in_left - first_in_left;
        int last1 = half + last_in_left - start;

        int first2 = end - last_in_right;
        int diff2 = second_in_right - first_in_right;
        int last2 = end - first_in_right;

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

int max_borderless_length_dbf(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }

    // at first check if word itself is borderless
    int * border_buffer = (int *) calloc(n, sizeof(int));
    border(text, border_buffer, n);
    if (border_buffer[n - 1] == 0) {
        return n;
    }

    DBF dbf(text, n);

    for (int i = border_buffer[n - 1]; i < n - 1; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            if (is_borderless(text, dbf, j, j + n - i)) {
                free(border_buffer);
                return n - i;
            }
        }
    }
    free(border_buffer);
    return 1;
}


int max_borderless_length_dbf_hashtable(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }

    // at first check if word itself is borderless
    int * border_buffer = (int *) calloc(n, sizeof(int));
    border(text, border_buffer, n);
    if (border_buffer[n - 1] == 0) {
        return n;
    }

    DBFHashTable dbf(text, n);

    for (int i = border_buffer[n - 1]; i < n - 1; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            if (is_borderless(text, dbf, j, j + n - i)) {
                free(border_buffer);
                return n - i;
            }
        }
    }
    free(border_buffer);
    return 1;
}

int max_borderless_length_border(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    for (int i = 0; i < n && n - i > max_len; ++i) {

        border(text + i, border_buffer, n - i);

        for (int j = n - i - border_buffer[n - i - 1]; j > max_len; j -= border_buffer[j - 1])
            if (border_buffer[j - 1] == 0)
                max_len = j;

    }
    free(border_buffer);
    return max_len;
}


int max_borderless_length_naive(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    for (int i = 0; i < n - 1 && n - i > max_len; ++i) {

        border(text + i, border_buffer, n - i);

        for (int j = n - i; j > max_len; --j)
            if (border_buffer[j - 1] == 0)
                max_len = j;

    }
    free(border_buffer);
    return max_len;
}


int max_borderless_length_super_naive(const char * text, int n) {
    if (n < 0) {
        n = (int) strlen(text);
    }
    int * border_buffer = (int *) calloc(n, sizeof(int));

    int max_len = 1;

    for (int i = 0; i < n - 1; ++i) {

        border(text + i, border_buffer, n - i);

        for (int j = n - i; j > i; --j)
            if (border_buffer[j - 1] == 0)
                max_len = j;

    }
    free(border_buffer);
    return max_len;
}
